.. index::
    single: Doctrine; Event listeners and subscribers

.. note::
    
    * 対象バージョン: 2.3以上
    * 翻訳更新日: 2014/05/13

.. _doctrine-event-config:

イベントリスナーとサブスクライバーを登録する
============================================

Doctrine は、リッチなイベントシステムをパッケージしており、システム内のほとんどの処理の際にイベントを投げます。つまり、自由に :doc:`サービス</book/service_container>` を作成して、 Doctrine に ``prePersist`` などのアクションがあったときに、知らせるようにすることができます。例えば、オブジェクトがデータベースに保存されたときに、その処理とは別に検索インデックスを作成するときなどに便利です。

Doctrine は、 Doctine イベントをリッスンできるオブジェクトの２つのタイプを定義しています。それは、リスナーとサブスクライバーです。両方とも似てますが、リスナーの方がわかりやすくなっています。詳細は、 Doctrine のサイトの `The Event System`_ を参照してください。

Doctrine のウェブサイトには、リッスンできる全てのイベントについても解説しています。

リスナー/サブスクライバーの設定
-------------------------------

イベントリスナーやサブスクライバーとして動作するサービスを登録するには、適切な名前の :ref:`tag<book-service-container-tags>` をつける必要があります。ユースケースに寄りますが、リスナーを全ての DBAL 接続や ORM エンティティマネージャにフックすることができます。また、特定の DBAL 接続やその接続を使用する全てのマネージャだけにフックすることもできます。

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                default_connection: default
                connections:
                    default:
                        driver: pdo_sqlite
                        memory: true

        services:
            my.listener:
                class: Acme\SearchBundle\Listener\SearchIndexer
                tags:
                    - { name: doctrine.event_listener, event: postPersist }
            my.listener2:
                class: Acme\SearchBundle\Listener\SearchIndexer2
                tags:
                    - { name: doctrine.event_listener, event: postPersist, connection: default }
            my.subscriber:
                class: Acme\SearchBundle\Listener\SearchIndexerSubscriber
                tags:
                    - { name: doctrine.event_subscriber, connection: default }

    .. code-block:: xml

        <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine">

            <doctrine:config>
                <doctrine:dbal default-connection="default">
                    <doctrine:connection driver="pdo_sqlite" memory="true" />
                </doctrine:dbal>
            </doctrine:config>

            <services>
                <service id="my.listener" class="Acme\SearchBundle\Listener\SearchIndexer">
                    <tag name="doctrine.event_listener" event="postPersist" />
                </service>
                <service id="my.listener2" class="Acme\SearchBundle\Listener\SearchIndexer2">
                    <tag name="doctrine.event_listener" event="postPersist" connection="default" />
                </service>
                <service id="my.subscriber" class="Acme\SearchBundle\Listener\SearchIndexerSubscriber">
                    <tag name="doctrine.event_subscriber" connection="default" />
                </service>
            </services>
        </container>

    .. code-block:: php
        
        use Symfony\Component\DependencyInjection\Definition;

        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'default_connection' => 'default',
                'connections' => array(
                    'default' => array(
                        'driver' => 'pdo_sqlite',
                        'memory' => true,
                    ),
                ),
            ),
        ));

        $container
            ->setDefinition(
                'my.listener',
                new Definition('Acme\SearchBundle\EventListener\SearchIndexer')
            )
            ->addTag('doctrine.event_listener', array('event' => 'postPersist'))
        ;
        $container
            ->setDefinition(
                'my.listener2',
                new Definition('Acme\SearchBundle\EventListener\SearchIndexer2')
            )
            ->addTag('doctrine.event_listener', array('event' => 'postPersist', 'connection' => 'default'))
        ;
        $container
            ->setDefinition(
                'my.subscriber',
                new Definition('Acme\SearchBundle\EventListener\SearchIndexerSubscriber')
            )
            ->addTag('doctrine.event_subscriber', array('connection' => 'default'))
        ;

リスナークラスの作成
--------------------

上記の例では、 ``my.listener`` と呼ばれるサービスが ``postPersist`` イベントで Doctrine リスナーとして設定されています。このサービスのクラスは、必ず ``postPersist`` メソッドを持っており、イベントが投げられたときに呼ばれます。

    // src/Acme/SearchBundle/Listener/SearchIndexer.php
    namespace Acme\SearchBundle\Listener;
    
    use Doctrine\ORM\Event\LifecycleEventArgs;
    use Acme\StoreBundle\Entity\Product;
    
    class SearchIndexer
    {
        public function postPersist(LifecycleEventArgs $args)
        {
            $entity = $args->getEntity();
            $entityManager = $args->getEntityManager();
            
            // "Product" エンティティの場合だけ何かをします
            if ($entity instanceof Product) {
                // Product を使って何かをします
            }
        }
    }

それぞれのイベントでは、 ``LifecycleEventArgs`` オブジェクトにアクセスができ、イベントのエンティティオブジェクトとエンティティマネージャにアクセスすることができます。

重要なこととして、リスナーは、アプリケーション内の *全て* のエンティティをリッスンすることを忘れないでください。つまり、エンティティの特定の種類の扱いのみを対象としたければ、上にあるようにメソッド内でエンティティのクラス名を調べる必要があります。例えば ``BlogPost`` ではなく、 ``Product`` エンティティを対象にしたいときなどです。

.. tip::

    Doctrine 2.4 からエンティティリスナーと呼ばれる機能が実装されました。
    これは一つの種類のエンティティだけに使われるライフサイクルリスナーです。
    詳しくは `the Doctrine Documentation`_ を参照してください。

サブスクライバークラスの作成
-----------------------------

Doctrine のイベントサブスクライバーは ``Doctrine\Common\EventSubscriber`` インターフェイスを実装する必要があります。また、サブスクライブするそれぞれのイベントに対応するイベントメソッドを備えなければなりません。

    // src/Acme/SearchBundle/EventListener/SearchIndexerSubscriber.php
    namespace Acme\SearchBundle\EventListener;

    use Doctrine\Common\EventSubscriber;
    use Doctrine\ORM\Event\LifecycleEventArgs;
    // Doctrine 2.4 では Doctrine\Common\Persistence\Event\LifecycleEventArgs;
    use Acme\StoreBundle\Entity\Product;

    class SearchIndexerSubscriber implements EventSubscriber
    {
        public function getSubscribedEvents()
        {
            return array(
                'postPersist',
                'postUpdate',
            );
        }

        public function postUpdate(LifecycleEventArgs $args)
        {
            $this->index($args);
        }

        public function postPersist(LifecycleEventArgs $args)
        {
            $this->index($args);
        }

        public function index(LifecycleEventArgs $args)
        {
            $entity = $args->getEntity();
            $entityManager = $args->getEntityManager();

            // おそらく ”Product" エンティティの場合だけ何かをします
            if ($entity instanceof Product) {
                // Product を使って何かをします
            }
        }
    }

.. tip::

    Doctrine のイベントサブスクライバは、 :ref:`Symfonyのイベントサブスクライバー <event_dispatcher-using-event-subscribers>` とは異なり、イベントに対して呼び出すメソッドを指定する配列を返すことができません。
    Doctrine のイベントサブスクライバは、単に、サブスクライブするイベント名の配列を返す必要があります。Doctrine はサブスクライブする各イベントと同じ名前のメソッドがサブスクライバに存在することを前提にして動くのです（イベントリスナを使うときと同じです）。

全ての機能について知りたければ、 Doctrine のドキュメントの `The Event System`_ の章を参照してください。

.. _`The Event System`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/events.html
.. _`the Doctrine Documentation`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/events.html#entity-listeners

.. 2012/01/04 ganchiku 9818ea3316d4fb8bb7e2a4fb4e7ffe777d05f2af
.. 2014/05/13 77web    65649aa712cee26d9b8a99b44313def11e42ab71

