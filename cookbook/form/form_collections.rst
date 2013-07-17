.. index::
   single: Form; Embed collection of forms

フォームのコレクションを埋め込む方法
====================================

この記事では、フォームのコレクションを埋め込んだフォームの作り方を学びます。これはとても便利で、例えば、 ``Task`` クラスがあり、 Task クラスに関連した ``Tag`` オブジェクトの 編集/登録/削除 をしたい際などに、同じフォーム内で行うことが可能になります。

.. note::

    この記事では、データベースの格納に Doctrine を使用することを想定しています。しかし、 Propel や他のデータベース接続など Doctrine を使用していなくても、方法はほとんど同じです。
    
    Doctrine を使用するために、 Task の ``tags`` プロパティに ``ManyToMany`` などの Doctrine 用のメタデータを追加する必要があります。

個々の ``Task`` が複数の ``Tags`` オブジェクトに属していることを前提として始めましょう。簡単な ``Task`` クラスを作成してください。
::

    // src/Acme/TaskBundle/Entity/Task.php
    namespace Acme\TaskBundle\Entity;
    
    use Doctrine\Common\Collections\ArrayCollection;

    class Task
    {
        protected $description;

        protected $tags;

        public function __construct()
        {
            $this->tags = new ArrayCollection();
        }
        
        public function getDescription()
        {
            return $this->description;
        }

        public function setDescription($description)
        {
            $this->description = $description;
        }

        public function getTags()
        {
            return $this->tags;
        }

        public function setTags(ArrayCollection $tags)
        {
            $this->tags = $tags;
        }
    }

.. note::

    この ``ArrayCollection`` は Doctrine に特化しており、基本的には ``array`` の使用と同じになります。しかし、ここでは ``ArrayCollection`` を使わなければいけません。

そして、 ``Tag`` クラスを作成します。上記にあるように ``Task`` はたくさんの ``Tag`` オブジェクトを持つことになります。
::

    // src/Acme/TaskBundle/Entity/Tag.php
    namespace Acme\TaskBundle\Entity;

    class Tag
    {
        public $name;
    }

.. tip::

    ``name`` プロパティは、 public にしてありますが、アクセスを簡単にするためだけで、 protected や private にしても構いません。その際は、アクセサの ``getName`` と ``setName`` メソッドが必要になります。

次にフォームを作成します。ユーザが ``Tag`` オブジェクトを変更することができるようにフォームクラスを作成します。
::

    // src/Acme/TaskBundle/Form/Type/TagType.php
    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;

    class TagType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('name');
        }

        public function getDefaultOptions(array $options)
        {
            return array(
                'data_class' => 'Acme\TaskBundle\Entity\Tag',
            );
        }

        public function getName()
        {
            return 'tag';
        }
    }

これで、タグフォームを表示させることができます。しかし、今回のゴールは、 ``Task`` のフォーム内で tags を変更できるようにすることです。 ``Task`` クラスを作成しましょう。

:doc:`collection</reference/forms/types/collection>` フィールドタイプを使用して ``TagType`` フォームのコレクションを埋め込むことを忘れないでください。
::

    // src/Acme/TaskBundle/Form/Type/TaskType.php
    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;

    class TaskType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('description');

            $builder->add('tags', 'collection', array('type' => new TagType()));
        }

        public function getDefaultOptions(array $options)
        {
            return array(
                'data_class' => 'Acme\TaskBundle\Entity\Task',
            );
        }

        public function getName()
        {
            return 'task';
        }
    }

これでコントローラで、 ``TaskType`` のインスタンスを初期化することができます。
::

    // src/Acme/TaskBundle/Controller/TaskController.php
    namespace Acme\TaskBundle\Controller;
    
    use Acme\TaskBundle\Entity\Task;
    use Acme\TaskBundle\Entity\Tag;
    use Acme\TaskBundle\Form\TaskType;
    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    
    class TaskController extends Controller
    {
        public function newAction(Request $request)
        {
            $task = new Task();
            
            // dummy code - Task がいくつか tag を持っているようにするためだけのダミーコードです
            // そのため、特別なことはしていません
            $tag1 = new Tag()
            $tag1->name = 'tag1';
            $task->getTags()->add($tag1);
            $tag2 = new Tag()
            $tag2->name = 'tag2';
            $task->getTags()->add($tag2);
            // end dummy code
            
            $form = $this->createForm(new TaskType(), $task);
            
            // ここで POST リクエストのフォーム処理を行います
            
            return $this->render('AcmeTaskBundle:Task:new.html.twig', array(
                'form' => $form->createView(),
            ));
        }
    }

これで対応するテンプレートで、 Task フォームの ``description`` とこの Task に既に関連している全てのタグの ``TagType`` フォームを表示できるようになりました。上記のコントローラでは、この動作を確認するためにダミーコードを追加してあります。 ``Task`` が作られた時点ではタグを１つも保持していないためです。

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Task/new.html.twig #}
        {# ... #}

        {# task の　description フィールドのみ表示します #}
        {{ form_row(form.description) }}

        <h3>Tags</h3>
        <ul class="tags">
            {# 既に関連しているタグをイテレートして name のみ表示します #}
			{% for tag in form.tags %}
            	<li>{{ form_row(tag.name) }}</li>
			{% endfor %}
        </ul>

        {{ form_rest(form) }}
        {# ... #}

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Task/new.html.php -->
        <!-- ... -->

        <h3>Tags</h3>
        <ul class="tags">
			<?php foreach($form['tags'] as $tag): ?>
            	<li><?php echo $view['form']->row($tag['name']) ?></li>
			<?php endforeach; ?>
        </ul>

        <?php echo $view['form']->rest($form) ?>
        <!-- ... -->

フォームが送信されたら、 ``Tags`` フィールドのデータは、 ``Tag`` オブジェクトの ArrayCollection を組み立てるのに使われます。この ArrayCollection は ``Task`` インスタンスの ``tag`` フィールドをセットします。

``Tags`` コレクションは、 ``$task->getTags()`` メソッドを使用してアクセスが可能になり、データベースに保存することができます。

これでちゃんと動きますが、動的な新しいタグを追加、既に関連している tag の削除はこのままではできません。現時点では tag を編集することはできますが、実際に追加する実装はしていません。

.. _cookbook-form-collections-new-prototype:

"prototype" として tag を"new" させる
-------------------------------------

このセクションはまだ執筆されていませんが、すぐにできるはずです。このセクションを執筆したい方は、 :doc:`/contributing/documentation/overview` を参照してください。

.. _cookbook-form-collections-remove:

tag を削除させる
----------------

このセクションはまだ執筆されていませんが、すぐにできるはずです。このセクションを執筆したい方は、 :doc:`/contributing/documentation/overview` を参照してください。

.. 2011/11/21 ganchiku 6ec385423860c428bac1fe1f7a1bd9f26e498efa

