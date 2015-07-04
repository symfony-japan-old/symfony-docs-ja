コントローラー
===========
Symfonyは薄いコントローラとファットなモデルという哲学を信奉します。 つまり、コントローラはアプリケーションの様々な部分をとりまとめるグルー(糊)コードだけからなる薄いレイヤーにするべきです。

だいたいの目安として、次の「5-10-20ルール」に従うとよいでしょう。
１コントローラで定義する変数を5個以下にすること、含まれるアクションを10個以下にすること、１アクションのコード行数を20行以下にすること。
この数字に科学的な根拠はありませんが、コントローラーからサービスへリファクタリングすべきタイミングを見極める基準としては役立ちます。


.. best-practice::

    コントローラを作る場合``FrameworkBundle``のベースコントローラを継承し、
    ルーティング、キャッシュ、セキュリティを設定するときはなるべくアノテーションを使ってください。


コントローラを基底フレームワークに結合させることで、フレームワークの機能すべてを
活用する事が可能になり、生産性が向上します。

コントローラは薄く数行のグルーコードであるべきです。何時間もかけてコントローラを
フレームワークから切り離そうとしても、長期的にはメリットがありません。
それに時間を費やしても何の価値もありません。

加えて、 ルーティング、キャッシュ、セキュリティに対してアノテーションを使えば設定がシンプルになります。
そうすれば、
YAML, XML, PHPなどのいろんなフォーマットで作られた何十もの設定ファイルを見る必要はなくなります。全ての設定は必要な場所に収まり、１つのフォーマットで済むでしょう。


あなたのビジネスロジックをフレームワークから切り離しながらも、
同時に、ルーティングとコントローラはフレームワークに積極的に結合するべきということです。
そうすればフレームワークを最大限活用できるでしょう。


ルーティングの設定
---------------------


コントローラでアノテーションとして定義されたルーティングを読み込むには、routing.ymlに以下の設定を追加します。

.. code-block:: yaml

    # app/config/routing.yml
    app:
        resource: "@AppBundle/Controller/"
        type:     annotation

このように設定すると、``src/AppBundle/Controller/`` ディレクトリとそのサブディレクトリにあるコントローラのアノテーションを読み込んでくれます。・
もしアプリケーションに多くのコントローラがある場合、それらを下図のようにサブディレクトリを作ってその中に移動しても全く問題ありません。


.. code-block:: text

    <your-project>/
    ├─ ...
    └─ src/
       └─ AppBundle/
          ├─ ...
          └─ Controller/
             ├─ DefaultController.php
             ├─ ...
             ├─ Api/
             │  ├─ ...
             │  └─ ...
             └─ Backend/
                ├─ ...
                └─ ...

テンプレート設定
----------------------

.. best-practice::

    コントローラから呼ばれるテンプレートの設定をするのに ``@Template()`` アノテーションを使用しないでください。

``@Template``は便利ですがある種の魔法を伴います。魔法を使うに値するとは思えないのでお勧めしません。

ほとんどの場合、``@Template``はパラメータなしで使われますが、そうするとどのテンプレートが呼ばれるのかわかりづらくなります。
また、コントローラは必ずレスポンスオブジェクトを返すべきだと言う事を初心者にわかりにくくします。(ビューレイヤーを使わない場合)

最後に、@Templateアノテーションはkernel.viewイベントのイベントディスパッチのフックとして
TemplateListenerクラスで利用されます。そのリスナーのパフォーマンスへの影響を紹介します。
ブログのサンプルアプリケーションのホームページをレンダリングする場合、$this->render()メソッドを
使った場合5ミリ秒かかり、@Templateアノテーションを使った場合26ミリ秒かかりました。

コントローラはこんな感じにしよう
------------------------

上記のことをふまえると、アプリケーションのホームページを表示するコントローラはこんな感じにするのがよいでしょう。

.. code-block:: php

    namespace AppBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;

    class DefaultController extends Controller
    {
        /**
         * @Route("/", name="homepage")
         */
        public function indexAction()
        {
            $posts = $this->getDoctrine()
                ->getRepository('AppBundle:Post')
                ->findLatest();

            return $this->render('default/index.html.twig', array(
                'posts' => $posts
            ));
        }
    }


.. _best-practices-paramconverter:

ParamConverterを使う
------------------------

Doctrineを使っている場合は`ParamConverter`_ を使うことができます。
これは自動的にエンティティを取得し、コントローラの引数にしてくれます。

.. best-practice::

    Doctrineのエンティティを自動的に取得してくれるParamConverterを使用
    してください。もしそれがシンプルかつ有用な場合は。

例:

.. code-block:: php

    use AppBundle\Entity\Post;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;

    /**
     * @Route("/{id}", name="admin_post_show")
     */
    public function showAction(Post $post)
    {
        $deleteForm = $this->createDeleteForm($post);

        return $this->render('admin/post/show.html.twig', array(
            'post'        => $post,
            'delete_form' => $deleteForm->createView(),
        ));
    }

通常は ``showAction`` では ``$id`` という変数を引数として使うと思います。
代わりに ``$post`` 引数と ``Post`` クラス(Doctrineのエンティティ)をタイプヒンティングする
ことによって、そのオブジェクトを自動的にParamConverterが``{id}`` の値と一致する
``$id`` プロパティのものを取得します。``Post`` が見つからなかった場合は404ページが表示されます。

高度な事
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This works without any configuration
This works without any configuration because the wildcard name ``{id}`` matches
the name of the property on the entity. If this isn't true, or if you have
even more complex logic, the easiest thing to do is just query for the entity
manually. In our application, we have this situation in ``CommentController``:

.. code-block:: php

    /**
     * @Route("/comment/{postSlug}/new", name = "comment_new")
     */
    public function newAction(Request $request, $postSlug)
    {
        $post = $this->getDoctrine()
            ->getRepository('AppBundle:Post')
            ->findOneBy(array('slug' => $postSlug));

        if (!$post) {
            throw $this->createNotFoundException();
        }

        // ...
    }

You can also use the ``@ParamConverter`` configuration, which is infinitely
flexible:

.. code-block:: php

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\ParamConverter;

    /**
     * @Route("/comment/{postSlug}/new", name = "comment_new")
     * @ParamConverter("post", options={"mapping": {"postSlug": "slug"}})
     */
    public function newAction(Request $request, Post $post)
    {
        // ...
    }

The point is this: the ParamConverter shortcut is great for simple situations.
But you shouldn't forget that querying for entities directly is still very
easy.

Pre and Post Hooks
------------------

If you need to execute some code before or after the execution of your controllers,
you can use the EventDispatcher component to `set up before/after filters`_.

.. _`ParamConverter`: http://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/annotations/converters.html
.. _`set up before/after filters`: http://symfony.com/doc/current/cookbook/event_dispatcher/before_after_filters.html
