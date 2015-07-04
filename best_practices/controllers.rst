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

通常、 ``showAction`` では ``$id`` という引数を期待するでしょう。その代わりに ``$post`` 引数を使い ``Post`` クラス(Doctrineのエンティティ)でタイプヒンティングすることで、
とによって、そのオブジェクトをParamConverterが自動的に``{id}`` の値と一致する``$id`` プロパティを持つオブジェクトを探してくれます。
また``Post`` が見つからなかった場合は404ページを表示してくれます。

When Things Get More Advanced
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

上記のコードが設定なしで動くのは、ワイルドカード名``{id}``がエンティティのプロパティ名に一致するからです。
もしそうでない場合、またはもっと複雑なロジックがある場合、これを実現する簡単な方法は手動でエンティティを取得することです。
本アプリケーションでは``CommentController``がその事例です。:

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

    use AppBundle\Entity\Post;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\ParamConverter;
    use Symfony\Component\HttpFoundation\Request;

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
you can use the EventDispatcher component to
:doc:`set up before and after filters </cookbook/event_dispatcher/before_after_filters>`.

.. _`ParamConverter`: https://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/annotations/converters.html
