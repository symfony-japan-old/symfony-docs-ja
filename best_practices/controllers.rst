コントローラー
===========
Symfonyは薄いコントローラとファットなモデルという哲学に従います。 つまり、コントローラは、
アプリケーションの様々な部分をとりまとめるグルーコードだけからなる薄いレイヤーにするべきです。

だいたいの目安として、次の「5-10-20ルール」に従うとよいでしょう。
１つのコントローラで定義する変数を5個以下にすること、含まれるアクションを10個以下にすること、１つのアクションのコード行数を20行以下にすること。
この数字に科学的な根拠はありませんが、コントローラーからサービスへリファクタリングすべきタイミングを見極める基準としては役立ちます。


.. best-practice::

    コントローラを作る場合``FrameworkBundle``のベースコントローラを継承し、
    ルーティング、キャッシュ、セキュリティを設定するときはなるべくアノテーションを使ってください。


コントローラを基底フレームワークに結合させることで、フレームワークの機能すべてを
活用する事が可能になり、生産性が向上します。

そして、コントローラーは薄く、数行のグルーコードであるべきで、
フレームワークからそれらを切り離すそうとすると、時間かかるので長期的に見ると利益になりません。
大量の時間を費やすだけで、何の価値もありません。

加えて、 ルーティング、キャッシュ、セキュリティに対してアノテーションを使用すると簡単に設定する事ができます。
すべての設定は必要な場所に一つのフォーマットにすれば、YAML, XML, PHPなどの複数ファイルを見る必要がなくなります。

全体的にビジネスロジックをフレームワークから切り離すという手段は積極的に行うと
同時に、最大限に活用にするためにルーティングとコントローラは結合するべきです。

ルーティング設定
---------------------

コントローラで定義されたアノテーションを利用するにはrouting.ymlに以下の設定を追加します。

.. code-block:: yaml

    # app/config/routing.yml
    app:
        resource: "@AppBundle/Controller/"
        type:     annotation

``src/AppBundle/Controller/`` ディレクトリとそのサブディレクトリからコントローラの
アノテーションをロードします。
もしアプリケーションに多くのコントローラがある場合、それらをサブディレクトリへ移動する事が可能です。

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

    テンプレートの設定は ``@Template()`` アノテーションを使用しないでください。

``@Template``は便利ですがいくつかの魔法を伴うので、お勧めする事はできません。

ほとんどの場合、``@Template``はパラメータの設定をせず使用されます。そして設定すると、どの
テンプレートを表示するかをわかりづらくします。それはまた、必ずコントローラはレスポンスオブジェクトを
返すべきだと言う事を初心者にわかりにくくしてしまいます。(あなたがビューレイヤーを利用している場合以外)

最後に、@Templateアノテーションはkernel.viewイベントのイベントディスパッチのフックとして
TemplateListenerクラスで利用されます。そのリスナーのパフォーマンスへの影響を紹介します。
ブログのサンプルアプリケーションのホームページをレンダリングする場合、$this->render()メソッドを
使った場合5ミリ秒かかり、@Templateアノテーションを使った場合26ミリ秒かかりました。

How the Controller Looks
------------------------

Considering all this, here is an example of how the controller should look
for the homepage of our app:

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
            $em = $this->getDoctrine()->getManager();
            $posts = $em->getRepository('App:Post')->findLatest();

            return $this->render('default/index.html.twig', array(
                'posts' => $posts
            ));
        }
    }

.. _best-practices-paramconverter:

ParamConverterを使う
------------------------

もしDoctrineを使っている場合は必要に応じて`ParamConverter`_ を使い、自動的にエンティティを取得し、
コントローラの引数として渡す必要があります。

.. best-practice::

    シンプルかつ簡単な場合は、自動的にDoctrineのエンティティを取得出来るParamConverterを使用
    してください。

例:

.. code-block:: php

    /**
     * @Route("/{id}", name="admin_post_show")
     */
    public function showAction(Post $post)
    {
        $deleteForm = $this->createDeleteForm($post);

        return $this->render('admin/post/show.html.twig', array(
            'post'      => $post,
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
