.. note::

    * 対象バージョン：2.3以降
    * 翻訳更新日：2014/10/23

フォーム
==========

フォームは、取り扱う範囲が広く、膨大な機能があるために、誤って使用されることが最も多いコンポーネントです。
この章では、フォームを活用して素早く実装できるようにすることができるベストプラクティスを説明します。

フォームを構築する
-------------------

.. best-practice::

    フォームをPHPクラスで定義しましょう。

Formコンポーネントでは、コントローラーのコードの中でもフォームを構築できるようになっています。はっきり言って、フォームをどこか他の場所で使う予定がないのなら、それでも全く構いません。
しかし、コードの整理と再利用のために、あらゆるフォームをPHPクラスとして定義することをおすすめします。

.. code-block:: php

    namespace AppBundle\Form;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class PostType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('title')
                ->add('summary', 'textarea')
                ->add('content', 'textarea')
                ->add('authorEmail', 'email')
                ->add('publishedAt', 'datetime')
            ;
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'data_class' => 'AppBundle\Entity\Post'
            ));
        }

        public function getName()
        {
            return 'post';
        }
    }

このフォームクラスを利用するためには、 ``createForm`` を使い、この新しいクラスのインスタンスを渡してください。

.. code-block:: php

    use AppBundle\Form\PostType;
    // ...

    public function newAction(Request $request)
    {
        $post = new Post();
        $form = $this->createForm(new PostType(), $post);

        // ...
    }

フォームをサービスとして登録する
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`フォームタイプをサービスとして定義する`_ こともできます。
しかし、多くの場所でフォームを再利用する予定があったり、他のフォームに直接あるいは `collection`_ として埋め込む予定がない限り、この方法は推奨 *しません* 。

ほとんどのフォームは、何かを編集したり作成したりするのに使われるだけなので、フォームをサービスとして登録するのはやりすぎです。コントローラーの中で使われているフォームクラスがどれなのかわかりにくくなってしまいます。

フォームのボタン定義
-------------------------

フォームクラスは、フォームがアプリケーション内の *どこで* 使われるのかなるべく無関心であるべきです。後から再利用しやすくなります。

.. best-practice::

    ボタンはテンプレート上で追加し、フォームクラスやコントローラーに書かないようにしましょう。

Symfony 2.5以降では、フォームにボタンの定義を追加することができます。フォームをレンダリングするテンプレートを簡略にするのに良い方法ですが、フォームクラスの中に直接ボタンを定義してしまうと、フォームの扱う範囲を極端に狭めてしまうことになるのです。

.. code-block:: php

    class PostType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                // ...
                ->add('save', 'submit', array('label' => 'Create Post'))
            ;
        }

        // ...
    }

このフォームは投稿を新規作成する用途で作られたの *かも* しれませんが、投稿を編集する場面で再利用しようとすると、ボタンのラベルが間違っていることになります。
その代わりに、コントローラーでフォームボタンを定義する開発者もいるでしょう。

.. code-block:: php

    namespace AppBundle\Controller\Admin;

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use AppBundle\Entity\Post;
    use AppBundle\Form\PostType;

    class PostController extends Controller
    {
        // ...

        public function newAction(Request $request)
        {
            $post = new Post();
            $form = $this->createForm(new PostType(), $post);
            $form->add('submit', 'submit', array(
                'label' => 'Create',
                'attr'  => array('class' => 'btn btn-default pull-right')
            ));

            // ...
        }
    }


これも重大な誤りです。というのも、プレゼンテーション用のマークアップ（ラベル、CSSクラスなど）を純粋なPHPコードの中に混在させてしまっているからです。
関心の分離は常に意識すべきプラクティスであり、全ての見た目に関係する物事はviewレイヤーに置くべきでしょう。

.. code-block:: html+jinja

    <form method="POST" {{ form_enctype(form) }}>
        {{ form_widget(form) }}

        <input type="submit" value="Create"
               class="btn btn-default pull-right" />
    </form>

フォームをレンダリングする
---------------------------

フォームをレンダリングする方法は、フォーム全体を一行でレンダリングするのから、フィールドの各パーツを個別にレンダリングするのまで、多岐に渡ります。
一番良い方法は、アプリケーションでどこまでのカスタマイズが必要かによって異なります。

一番シンプルな方法（特に開発中に便利な方法）はHTMLのフォームタグを書いてから、 ``form_widget()`` を使って全てのフィールドを一度にレンダリングする方法です。

.. code-block:: html+jinja

    <form method="POST" {{ form_enctype(form) }}>
        {{ form_widget(form) }}
    </form>

.. best-practice::

    フォームの開始タグや終了タグに ``form()`` や ``form_start()`` を使わないようにしましょう。

Symfony に慣れた開発者なら、 ``<form>`` タグを ``form_start()`` や ``form()`` といったヘルパーを使わずにレンダリングしていることに気づくでしょう。
ヘルパーは便利ですが、反面、僅かな利便性と引き換えにわかりやすさを損なっているのです。

.. tip::

    削除フォームは例外です。削除フォームはただ一つだけのボタンなので、ショートカットによる利便性を選択しても良いでしょう。

もしフィールドのレンダリング内容についてもっとコントロールしたければ、 ``form_widget(form)`` を削除してフィールドを個別にレンダリングしたほうが良いでしょう。
フィールドを個別にレンダリングする方法の詳細と、フォームテーマを使ってフォームレンダリングをアプリケーション全体でカスタマイズする方法については `How to Customize Form Rendering`_ を参考にしてください。

フォーム送信を扱う
---------------------

フォーム送信を受け取る処理は、大抵の場合、一種の定型文になります。

.. code-block:: php

    public function newAction(Request $request)
    {
        // フォームを構築 ...

        $form->handleRequest($request);

        if ($form->isSubmitted() && $form->isValid()) {
            $em = $this->getDoctrine()->getManager();
            $em->persist($post);
            $em->flush();

            return $this->redirect($this->generateUrl(
                'admin_post_show',
                array('id' => $post->getId())
            ));
        }

        // テンプレートをレンダリング
    }

大事なことは2つだけです。まず、フォームのレンダリングとフォーム送信の受付に一つのアクションを使いましょう。
例えば、フォームのレンダリング *だけ* を行う ``newAction`` とフォーム送信の受付だけを行う ``createAction`` を両方作ったとします。この2つのアクションはほとんど同じ内容になるでしょう。ならば、 ``newAction`` に全てを処理させるほうがもっとシンプルになります。

次に、わかりやすくするために ``if`` 条件文に ``$form->isSubmitted()`` を使いましょう。
``isValid()`` は内部的に最初に ``isSubmitted()`` を呼び出しているので、技術的には必要のないことです。しかし、これがなければ、一見してフォーム送信を受け付ける処理が *常に* 行われるように（GETリクエストの時であっても）見えてしまい、処理の流れが読み取りにくくなってしまうのです。

.. _`フォームをサービスとして登録する`: http://symfony.com/doc/current/cookbook/form/create_custom_field_type.html#creating-your-field-type-as-a-service
.. _`collection`: http://symfony.com/doc/current/reference/forms/types/collection.html
.. _`How to Customize Form Rendering`: http://symfony.com/doc/current/cookbook/form/form_customization.html
.. _`form event system`: http://symfony.com/doc/current/cookbook/form/dynamic_form_modification.html

.. 2014/10/23 77web 2c2000a0274b182cbf1a429badb567ee65432c54
