.. index::
   single: Form; Events

フォームイベントを使用してフォームを動的に生成する方法
======================================================

動的なフォーム生成に入る前に、初期状態のフォームクラスがどうなっているか見てみましょう。
::

    //src/Acme/DemoBundle/Form/ProductType.php
    namespace Acme\DemoBundle\Form

    use Symfony\Component\Form\AbstractType
    use Symfony\Component\Form\FormBuilder;
    
    class ProductType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('name');
            $builder->add('price');
        }

        public function getName()
        {
            return 'product';
        }
    }

.. note::

    上記の記事のコードが理解できない方は、先に :doc:`Forms chapter </book/forms>`  を参照してから、この記事を読んでください。

仮想的な "Product" クラスを使用したフォームがあることを想定してください。 ``Product`` クラスには、 "name" と "price" の２つのプロパティがあるとします。このクラスによって生成されたフォームは、新しく Product を作成する際にも、データベース等で保存されている内容を編集する際にも、同じものが使われます。

オブジェクトを作成した後の編集の際に `name` の値を変更させたくないとしましょう。そうするには、Symfony のイベントディスパッチャー  :ref:`Event Dispatcher <book-internals-event-dispatcher>` を使用してオブジェクトのデータを解析して、 Product のデータに基づいたフォームを変更することができます。この記事では、そのフォームの柔軟性について学びます。

.. _`cookbook-forms-event-subscriber`:

フォームクラスにイベントサブスライバ(Event Subscriber)を追加する
----------------------------------------------------------------

ProductType のフォームクラスに "name" ウィジェットを追加する代わりに、イベントサブスクライバ(Event Subscriber)にフィールド作成の責任を移譲しましょう。
::

    //src/Acme/DemoBundle/Form/ProductType.php
    namespace Acme\DemoBundle\Form

    use Symfony\Component\Form\AbstractType
    use Symfony\Component\Form\FormBuilder;
    use Acme\DemoBundle\Form\EventListener\AddNameFieldSubscriber;

    class ProductType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $subscriber = new AddNameFieldSubscriber($builder->getFormFactory());
            $builder->addEventSubscriber($subscriber);
            $builder->add('price');
        }

        public function getName()
        {
            return 'product';
        }
    }

イベントサブスライバは、 FormFactory オブジェクトのコンストラクタに渡されるので、このサブスクライバは、フォーム作成時にディスパッチされたイベントを通知されたときにフォームウィジェットを作成することができます。

.. _`cookbook-forms-inside-subscriber-class`:

イベントサブスライバ(Event Subscriber)クラスの内部
--------------------------------------------------

まだデータベースに保存されていないときなど、フォーム内の Product オブジェクトが新規のとき *のみ* "name" フィールドを作成することが今回のゴールです。
::

    // src/Acme/DemoBundle/Form/EventListener/AddNameFieldSubscriber.php
    namespace Acme\DemoBundle\Form\EventListener;

    use Symfony\Component\Form\Event\DataEvent;
    use Symfony\Component\Form\FormFactoryInterface;
    use Symfony\Component\EventDispatcher\EventSubscriberInterface;
    use Symfony\Component\Form\FormEvents;

    class AddNameFieldSubscriber implements EventSubscriberInterface
    {
        private $factory;
        
        public function __construct(FormFactoryInterface $factory)
        {
            $this->factory = $factory;
        }
        
        public static function getSubscribedEvents()
        {
            // ディスパッチャに form.pre_set_data イベントをリッスンして
            // preSetData メソッドが呼ばれるように伝えます
            return array(FormEvents::PRE_SET_DATA => 'preSetData');
        }

        public function preSetData(DataEvent $event)
        {
            $data = $event->getData();
            $form = $event->getForm();
            
            // フォーム作成時に FormBuilder のコンストラクタによって setData() は null の引数で呼ばれます。
            // Doctrine に新規に保存するとき、またはデータを取ってきたときなど
            // 実際のエンティティオブジェクトを操作する際の setData() のみを対象にします。
            // そのため、この if 文は null の条件をスキップさせます。
            if (null === $data) {
                return;
            }

            // Product オブジェクトが "new" かどうか調べます
            if (!$data->getId()) {
                $form->add($this->factory->createNamed('text', 'name'));
            }
        }
    }

.. caution::

    このイベントディスパッチャの ``if (null === $data)`` 部の目的がよく間違って理解されます。正しく理解するために、 `Form class`_ の中を参照し、特にコンストラクタの最後で ``setData()`` メソッドが呼ばれるところと ``setData()`` メソッド自体に注目してください。

``FormEvents::PRE_SET_DATA`` の行は実際に ``form.pre_set_data`` 文字列となります。 `FormEvent class`_ は構造上の目的を担います。 `FormEvent class`_ は、中央の場所となり、そこで利用可能ないろんなフォームイベントを探すことができます。

今回の例では、 ``form.set_data`` イベントや ``form.post_set_data`` イベントを使用することができました。しかし、 ``form.pre_set_data`` を使用することで、 ``Event`` オブジェクトから取り出したデータが他のサブスライバやリスナによって変更されることがないということを保証することができる点で違います。なぜなら ``form.pre_set_data`` は `form.set_data`` イベントによって渡される ``FilterDataEvent`` オブジェクトではなく、 `DataEvent`_ オブジェクトを渡すからです。 `DataEvent`_ は `FilterDataEvent`_ の親クラスで setData() メソッドはありませんので、サブスライバやリスナによって変更されることはないのです。

.. note::

    フォームイベントの完全な一覧は `FormEvents class`_ を参照してください。

.. _`DataEvent`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Event/DataEvent.php
.. _`FormEvents class`: https://github.com/symfony/Form/blob/master/FormEvents.php
.. _`Form class`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Form.php
.. _`FilterDataEvent`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Event/FilterDataEvent.php

.. 2011/11/20 ganchiku 4068ee50c31fd31acb030d6773718b66d167fbf2

