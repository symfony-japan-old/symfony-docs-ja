.. index::
   single: Routing

.. note::

    * 対象バージョン：2.4 (2.2以降)
    * 翻訳更新日：2014/01/04

ルーティング
============

Web アプリケーションでは、``index.php?article_id=57`` ではなく ``/read/intro-to-symfony`` のような URL を使うことが一般的となっています。これはルーティングシステムの 1 つの機能です。

また、ルーティングシステムを使うとアプリケーションに対して URL が柔軟になります。\
例えば、あるページの URL を、\ ``/blog`` から ``/news`` に変更する場合を考えてみてください。\
アプリケーション中に記述されたリンクの箇所を探し出して、1 つずつ変更する必要があるでしょうか。\
Symfony のルーターを使えば、変更は簡単です。

Symfony2 ルーターでは、URL をいくつも定義でき、それらをアプリケーション内の様々な要素にマップすることができます。\
この章では、次の事項を解説します。

* 複雑なルートの、コントローラーへのマッピング
* テンプレートやコントローラーにおける URL の生成
* バンドル (もしくはそれ以外でも) から、ルーティングリソースのロード
* ルートのデバッグ

.. index::
   single: Routing; Basics

実例でみるルーティング
----------------------

*ルート* の意味するところは、URL のパスとコントローラーとのマップです。\
例えば、\ ``/blog/my-post`` や ``/blog/all-about-symfony`` のような URL にマッチしたときに、\
コントローラーに送り、該当するブログエントリを探してレンダリングさせたいとしましょう。\
このルートはシンプルです。

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        blog_show:
            path:      /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" path="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog_show', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

.. versionadded:: 2.2
    ``path`` オプションは Symfony 2.2 以降で利用可能です。
    これ以前のバージョンでは ``pattern`` オプションでした。

ルート ``blog_show`` のパスは ``/blog/*`` のような URL に該当し、\
ワイルドカード部分には ``slug`` という名前が与えられています。\
``/blog/my-blog-post`` という URL では、変数 ``slug`` が ``my-blog-post`` という\
値となり、コントローラー上でも使用することができます(後述します)。

``_controller`` パラメーターには、URL がマッチした際に実行されるコントローラーを指定します。\
``_controller`` の文字列は\ :ref:`論理名 <controller-string-syntax>`\ で、\
あるパターンに従って、特定の PHP クラスやメソッドを指定します。

.. code-block:: php

    // src/Acme/BlogBundle/Controller/BlogController.php
    namespace Acme\BlogBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class BlogController extends Controller
    {
        public function showAction($slug)
        {
            // $slug 変数を使ってデータベースに問い合わせる
            $blog = ...;
            
            return $this->render('AcmeBlogBundle:Blog:show.html.twig', array(
                'blog' => $blog,
            ));
        }
    }

これでルートがひとつ定義でき、コントローラーにマップされました。\
``/blog/my-post`` に誰かがアクセスしたら、 ``showAction`` コントローラーが実行され、\
変数 ``$slug`` は ``my-post`` を意味するようになります。

Symfony2 ルーターの目標は、リクエストの URL をコントローラーにマップすることです。\
様々なトリックを使って、どんなに複雑な URL でも簡単にマッピングしていく方法をマスターしていきましょう。

.. index::
   single: Routing; Under the hood

ルーティング: その裏では
------------------------

アプリケーション上でリクエストが作成されたら、そのリクエストには、\
クライアントが必要としている「リソース」へのアドレスが含まれています。\
このアドレスのことを URL(URI) と言い、\ ``/contact`` や ``/blog/read-me`` のようになります。\
次のような HTTP リクエストを見てみましょう。

.. code-block:: text

    GET /blog/my-blog-post

Symfony2 ルーティングシステムの目標は、この URL をパースし、\
どのコントローラーを実行するべきか決定することです。\
このプロセスは次のような流れになります。

#. Symfony2 フロントコントローラー(\ ``app.php``)によって、リクエストが処理される

#. Symfony2 のコア(Kernel) がルーターにリクエストを調べるよう依頼する

#. ルーターは、URL とルートをマッチさせ、マッチしたルートの情報を返す。
   この情報には、どのコントローラーが実行されるべきか、という情報が含まれている

#. Kernel が、特定されたコントローラーを実行する。コントローラーは最終的には ``Response`` オブジェクトを返す

.. figure:: /images/request-flow.png
   :align: center
   :alt: Symfony2 リクエストフロー

   ルーティング層は、URL を、実行すべきコントローラーに変換するツール

.. index::
   single: Routing; Creating routes

ルートを定義する
----------------

Symfony は、アプリケーション上のすべてのルートを、単一の設定ファイルからロードします。\
通常そのファイルは、\ ``app/config/routing.yml`` ですが、\
アプリケーションの設定ファイルを通じて、如何様にも(XMLやPHPファイルを含む) 設定できます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            router:        { resource: "%kernel.root_dir%/config/routing.yml" }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config>
                <!-- ... -->
                <framework:router resource="%kernel.root_dir%/config/routing.xml" />
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'router' => array(
                'resource' => '%kernel.root_dir%/config/routing.php',
            ),
        ));

.. tip::

    ルートがロードされる起点となるのは単一のファイルですが、\
    通常はバンドルごとに独立したファイルにルートを定義し、起点となるファイルからインクルードします。\
    詳細は、\ :ref:`routing-include-external-resources` 節を参照してください。

基本的なルートの設定
~~~~~~~~~~~~~~~~~~~~

ルートの定義は簡単ですが、典型的なアプリケーションでは多数のルートをもつことになるでしょう。\
ルートは、基本的には 2 つのパート、すなわち、マッチさせるための ``path`` と ``defaults`` 配列からなります。

.. configuration-block::

    .. code-block:: yaml

        _welcome:
            path:      /
            defaults:  { _controller: AcmeDemoBundle:Main:homepage }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">
            <route id="_welcome" path="/">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
            </route>

        </routes>

    ..  code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('_welcome', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
        )));

        return $collection;

このルートは、トップページ (``/``) にマッチし、\ ``AcmeDemoBundle:Main:homepage`` コントローラーにマップされています。\
``_controller`` 部分の文字列は、Symfony2 によって実際の PHP 的関数に変換されて、実行されます。\
この動きに関しては、 :ref:`controller-string-syntax` 節で説明します。

.. index::
   single: Routing; Placeholders

プレースホルダー付きのルート
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

もちろん、もっと面白いルートもサポートしています。\
多くのルートでは、「ワイルドカード」と呼ばれるプレースホルダーを、1 つ以上含むことになるでしょう。

.. configuration-block::

    .. code-block:: yaml

        blog_show:
            path:      /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">
            <route id="blog_show" path="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog_show', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

このパスでは、\ ``/blog/*`` のような URL には全てマッチします。\
加えて、コントローラー内では、プレースホルダー ``{slug}`` にマッチした値が使用できます。\
``/blog/hello-world`` という URL の場合、変数 ``$slug`` には ``hello-world`` という値が割り当てられ、\
コントローラー内で使用可能です。\
この値を利用してブログエントリを読み込めます。

ただし、このパスでは、単に ``/blog`` という URL にはマッチ\ *しません*\ 。\
なぜなら、全てのプレースホルダーはデフォルトでは必須となっているからです。\
これを避けるには、\ ``defaults`` 配列にプレースホルダーの値を与えておくとよいでしょう。

プレースホルダーの必須/任意設定
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

話をおもしろくするために、この仮ブログアプリケーションに、全エントリのリストを表示するルートを追加してみましょう。

.. configuration-block::

    .. code-block:: yaml

        blog:
            path:      /blog
            defaults:  { _controller: AcmeBlogBundle:Blog:index }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" path="/blog">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
        )));

        return $collection;

この時点では、ルートは一番シンプルな形です。\
プレースホルダーも入っていないので、\ ``/blog`` という URL にだけマッチすることになります。\
それでは、このルートが、ページネーションをサポートしたいとしたらどうでしょう。\
``/blog/2`` で 2 ページ目を表示させたいとします。\
\ ``{page}`` プレースホルダーを足してみましょう。

.. configuration-block::

    .. code-block:: yaml

        blog:
            path:      /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" path="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
        )));

        return $collection;

先程の\ ``{slug}`` と同様に、\ ``{page}`` 部分にマッチした値は、コントローラー内で使うことができます。\
そのページで、どのブログエントリ群を表示するかを決定するために使用すればいいのです。

プレースホルダーはデフォルトでは必須なので、このルートでは、もはや単純な ``/blog`` にはマッチしなくなってしまいます。\
1 ページ目を見たければ、代わりに、\ ``/blog/1`` という URL にする必要があります！\
とはいえ、これはリッチな Web アプリではあり得ない話なので、\
``{page}`` パラメーターは任意としましょう。\
任意にするには、\ ``defaults`` に追加します。

.. configuration-block::

    .. code-block:: yaml

        blog:
            path:      /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" path="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page'        => 1,
        )));

        return $collection;

``defaults`` に ``page`` を追加すると、\ ``{page}`` は必須ではなくなります。\
こうしておくと、このルートに URL ``/blog`` がマッチするようになります。\
その場合、\ ``page`` パラメータは ``1`` にセットされます。\
もちろん ``/blog/2`` にもマッチし、その場合の ``page`` は ``2`` になります。\

+--------------------+--------+-----------------------+
| URL                | ルート | パラメータ            |
+====================+========+=======================+
| /blog              | blog   | {page} = 1            |
+--------------------+--------+-----------------------+
| /blog/1            | blog   | {page} = 1            |
+--------------------+--------+-----------------------+
| /blog/2            | blog   | {page} = 2            |
+--------------------+--------+-----------------------+

.. tip::

    パスの末尾に必須ではないパラメーターのあるルートは、末尾にスラッシュのある URL のリクエストにはマッチしません
    (``/blog/`` ではマッチせず ``/blog`` にはマッチします)

.. index::
   single: Routing; Requirements

ルートの条件 (``requirements``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

今までのルートをまとめて見てみましょう。

.. configuration-block::

    .. code-block:: yaml

        blog:
            path:      /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }

        blog_show:
            path:      /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" path="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
            </route>

            <route id="blog_show" path="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page'        => 1,
        )));

        $collection->add('blog_show', new Route('/blog/{show}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

このルート定義には問題があります。
2 つのルートが ``/blog/*`` のような URL をマッチさせてしまうパターンになっています。\
Symfony のルータでは、\ **最初**\ にマッチしたルートが適用されます。\
したがって、この例では ``blog_show`` ルートには\ *絶対に*\ マッチしません。\
``/blog/my-blog-post`` のような URL でも、最初のルート (``blog``) にマッチしてしまい、\
``{page}`` には意味をなさない ``my-blog-post`` という値が返されてしまいます。

+--------------------+--------+-----------------------+
| URL                | ルート | パラメーター          |
+====================+========+=======================+
| /blog/2            | blog   | {page} = 2            |
+--------------------+--------+-----------------------+
| /blog/my-blog-post | blog   | {page} = my-blog-post |
+--------------------+--------+-----------------------+

この問題の解は、\ *requirements* や *conditions* (:ref:`book-routing-conditions` を参照) を追加することです。\
今回の例では、パスである ``/blog/{page}`` には、\ ``{page}`` 部分が数字である URL に\ *のみ*\ マッチすればうまくいくはずです。\
各パラメーターに、正規表現の requirements を追加しましょう。

.. configuration-block::

    .. code-block:: yaml

        blog:
            path:      /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }
            requirements:
                page:  \d+

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" path="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
                <requirement key="page">\d+</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page'        => 1,
        ), array(
            'page' => '\d+',
        )));

        return $collection;

``\d+`` の部分は、パラメータ ``{page}`` が数字でなければならない、という正規表現です。\
ルート ``blog`` は、\ ``/blog/2`` のような URL にはマッチします (2 は数字だから) が、\
``/blog/my-blog-post`` のような URL にはもうマッチしません(``my-blog-post`` は\ *数字ではない*\ から)。

こうして、\ ``/blog/my-blog-post`` のような URL が、正しくルート ``blog_show`` にマッチすることになります。

+--------------------+------------+-----------------------+
| URL                | ルート     | パラメーター          |
+====================+============+=======================+
| /blog/2            | blog       | {page} = 2            |
+--------------------+------------+-----------------------+
| /blog/my-blog-post | blog_show  | {slug} = my-blog-post |
+--------------------+------------+-----------------------+

.. sidebar:: 最初が勝つ

    これの意味することは、ルートの順番がとても重要だということです。\
    ルート ``blog_show`` が、ルート ``blog`` より上に位置していれば、\
    ``/blog/2`` は、\ ``blog`` ではなく、\ ``blog_show`` にマッチするでしょう。\
    どうしてかというと、\ ``blog_show`` の ``{slug}``  には、\
    requirements が設定されていないからです。\
    正しい順番で、うまく requirements を使用していくことで、大概の事は可能になります。

requirements は正規表現なので、複雑さ、柔軟さという点では、完全にあなた次第です。\
それでは、URL に応じて 2 つの言語が有効なアプリケーションを考えてみましょう。

.. configuration-block::

    .. code-block:: yaml

        homepage:
            path:      /{culture}
            defaults:  { _controller: AcmeDemoBundle:Main:homepage, culture: en }
            requirements:
                culture:  en|fr

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="homepage" path="/{culture}">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
                <default key="culture">en</default>
                <requirement key="culture">en|fr</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('homepage', new Route('/{culture}', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
            'culture'     => 'en',
        ), array(
            'culture' => 'en|fr',
        )));

        return $collection;

リクエストに対して、URL の ``{culture}`` の部分は、\ ``(en|fr)`` という正規表現が適用されます。

+-----+--------------------------+
| /   | {culture} = en           |
+-----+--------------------------+
| /en | {culture} = en           |
+-----+--------------------------+
| /fr | {culture} = fr           |
+-----+--------------------------+
| /es | *マッチしない*           |
+-----+--------------------------+

.. index::
   single: Routing; Method requirement

HTTP メソッド の制約をつける
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

マッチさせる部分は、URL に加えて、\ *メソッド*\ (GET, HEAD, POST, PUT, DELETE)があります。\
2 つのコントローラーからなるお問い合わせフォームを考えてみましょう。\
1 つは、GET リクエストでフォームを表示するコントローラー、\
もう 1 つは、POST リクエストで送信されたフォームを処理するコントローラーです。\
次のようなルート設定で、これが実現可能になります。

.. configuration-block::

    .. code-block:: yaml

        contact:
            path:     /contact
            defaults: { _controller: AcmeDemoBundle:Main:contact }
            methods:  [GET]

        contact_process:
            path:     /contact
            defaults: { _controller: AcmeDemoBundle:Main:contactProcess }
            methods:  [POST]

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="contact" path="/contact" methods="GET">
                <default key="_controller">AcmeDemoBundle:Main:contact</default>
            </route>

            <route id="contact_process" path="/contact" methods="POST">
                <default key="_controller">AcmeDemoBundle:Main:contactProcess</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contact',
        ), array(), array(), '', array(), array('GET')));

        $collection->add('contact_process', new Route('/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contactProcess',
        ), array(), array(), '', array(), array('POST')));

        return $collection;

.. versionadded:: 2.2
    ``methods`` オプションは Symfony 2.2 で追加されました。
    以前のバージョンでは ``requirements`` オプションの ``_method`` を使ってください。

この2つのルートは全く同じパス (``/contact``) となってはいるのですが、\
最初のルートは、GET リクエストにのみマッチするのに対して、\
二つ目のルートは、POST リクエストにのみマッチします。\
これの意味するところは、同一の URL でフォームと送信を表示することが可能で、かつ、\
異なる2つのコントローラーを使い分けることが可能になる、ということです。

.. note::

    ``method`` が指定されていない場合は、\ *すべて*\ のメソッドがマッチします。

ホスト制約をつける
~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.2
    ホスト制約は Symfony 2.2 以降で利用可能です。

リクエストの HTTP *ホスト* をルートのマッチ条件に追加できます。
詳細は、Routing コンポーネントの :doc:`/components/routing/hostname_pattern` を参照してください。

.. _book-routing-conditions:

制約付きルートマッチの完全なカスタマイズ例
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.4
    ルートの ``condition`` オプションは Symfony 2.4 以降で利用可能です。

ルートは、特定のルーティング正規表現ワイルドカード、HTTP メソッド、ホスト名でマッチングします。
``conditions`` を使うと、式言語を使ってマッチング条件をさらに柔軟に指定できるようになります。

.. configuration-block::

    .. code-block:: yaml

        contact:
            path:     /contact
            defaults: { _controller: AcmeDemoBundle:Main:contact }
            condition: "context.getMethod() in ['GET', 'HEAD'] and request.headers.get('User-Agent') matches '/firefox/i'"

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="contact"
                path="/contact"
                condition="context.getMethod() in ['GET', 'HEAD'] and request.headers.get('User-Agent') matches '/firefox/i'"
            >
                <default key="_controller">AcmeDemoBundle:Main:contact</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route(
            '/contact', array(
                '_controller' => 'AcmeDemoBundle:Main:contact',
            ),
            array(),
            array(),
            '',
            array(),
            array(),
            'context.getMethod() in ["GET", "HEAD"] and request.headers.get("User-Agent") matches "/firefox/i"'
        ));

        return $collection;

``condition`` は式で指定します。式の構文については
:doc:`/components/expression_language/syntax` を参照してください。
上の例のルートでは、HTTP メソッドが GET または HEAD で、かつ、\ ``User-Agent`` ヘッダーに ``firefox`` が含まれる場合にのみルートにマッチします。

式の中で次の 2 つの変数を使うことで、複雑な条件を組み立てることができます。

* ``context``: マッチしたルートに関する基礎的な情報を含む、\ :class:`Symfony\\Component\\Routing\\RequestContext` クラスのインスタンス
* ``request``: Symfony の :class:`Symfony\\Component\\HttpFoundation\\Request`
  オブジェクト (:ref:`component-http-foundation-request` を参照)

.. caution::

    ``conditions`` オプションは、URL の生成処理では加味されません。

.. sidebar:: 式は PHP にコンパイルされる

    この仕組の背後では、式は PHP にコンパイルされます。
    今回の例では、キャッシュディレクトリに次のような PHP コードが生成されます。

    .. code-block:: php

        if (rtrim($pathinfo, '/contact') === '' && (
            in_array($context->getMethod(), array(0 => "GET", 1 => "HEAD"))
            && preg_match("/firefox/i", $request->headers->get("User-Agent"))
        )) {
            // ...
        }

    実行時にはこの PHP コードが処理されるだけなので、\ ``condition`` キーの利用による処理時間のオーバーヘッドはありません。

.. index::
   single: Routing; Advanced example
   single: Routing; _format parameter

.. _advanced-routing-example:

高度な例
~~~~~~~~

ここまでで、Symfony のパワフルなルーティング構造を構成するのに必要なことがすべてわかりました。\
それでは、次の例で、ルーティングのシステムがどれだけ柔軟性に富んでいるのかを見ていきましょう。

.. configuration-block::

    .. code-block:: yaml

        article_show:
          path:     /articles/{culture}/{year}/{title}.{_format}
          defaults: { _controller: AcmeDemoBundle:Article:show, _format: html }
          requirements:
              culture:  en|fr
              _format:  html|rss
              year:     \d+

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="article_show"
                path="/articles/{culture}/{year}/{title}.{_format}">
                <default key="_controller">AcmeDemoBundle:Article:show</default>
                <default key="_format">html</default>
                <requirement key="culture">en|fr</requirement>
                <requirement key="_format">html|rss</requirement>
                <requirement key="year">\d+</requirement>

            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add(
            'homepage',
            new Route('/articles/{culture}/{year}/{title}.{_format}', array(
                '_controller' => 'AcmeDemoBundle:Article:show',
                '_format'     => 'html',
            ), array(
                'culture' => 'en|fr',
                '_format' => 'html|rss',
                'year'    => '\d+',
            ))
        );

        return $collection;

ご覧のとおり、このルートでは、URL の ``{culture}`` の部分が ``en`` か ``fr`` の場合で、\
かつ、\ ``{year}`` が数字の場合にのみマッチします。\
また、プレースホルダ間で、スラッシュとは別に、ピリオドを使用する方法も示しています。\
このルートでは下記のような URL がマッチします。

* ``/articles/en/2010/my-post``
* ``/articles/fr/2010/my-post.rss``
* ``/articles/en/2013/my-latest-post.html``

.. _book-routing-format-param:

.. sidebar:: ルーティングパラメータ ``_format``

    この例では、ルーティングパラメータ ``_format`` も目玉部分です。\
    このパラメータを使うと、マッチした値は ``Request`` オブジェクトの\
    「リクエストフォーマット」になります。\
    最終的には、このリクエストフォーマットは、レスポンスの ``Content-Type`` 設定に使用されたりします。\
    例えば、\ ``json`` は、``Content-Type`` では ``application/json`` に変換されます。\
    また、コントローラー内で、 ``_format`` の値に応じて、異なるテンプレートを出力するためにも使用されます。\
    ``_format`` パラメータは、同一のコンテンツを異なるフォーマットで出力する、\
    とても強力な方法です。

.. note::

    ルートの一部分をグローバルに設定できるようにしたい場合もあります。
    この場合は、Symfony サービスコンテナのパラメーターを使います。
    詳細は :doc:`/cookbook/routing/service_container_parameters` を参照してください。

特殊なルーティングパラメーター
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ここまでの例で見てきたように、ルートパラメーターやルートのデフォルト値は、コントローラーメソッドの引数を通して利用できました。
同じように使える特殊なパラメーターが 3 つあります。
アプリケーションで便利に使えるパラメーターです。

* ``_controller``: マッチしたルートで実行されるコントローラー

* ``_format``: リクエストフォーマットを設定する (:ref:`read more <book-routing-format-param>`);

* ``_locale``: リクエストのロケールを設定する (:ref:`read more <book-translation-locale-url>`);

.. tip::

    ルートで ``_locale`` パラメーターを使うと、指定したロケールの値がセッションにも保存され、後続のリクエストで同じロケールを維持できます。

.. index::
   single: Routing; Controllers
   single: Controller; String naming format

.. _controller-string-syntax:

コントローラーの指定パターン
----------------------------

ルートには、\ ``_controller`` パラメーターを指定する必要があります。\
ルートがマッチすると、このパラメータによって、実行されるコントローラーが決定されます。\
``_controller`` パラメーターに指定する値は\ *論理コントローラー名*\ で、シンプルな文字列パターンに従っており、\
Symfony が特定の PHP メソッド・クラスにマッピングする際に使用されます。\
論理コントローラー名は、次の 3 つの部分で構成され、それぞれコロンで区切られます。

    **バンドル**:**コントローラー**:**アクション**

たとえば、\ ``_controller`` の値が ``AcmeBlogBundle:Blog:show`` となっている場合は、次のような意味になります。

+----------------+------------------------+-------------+
| バンドル       | コントローラーのクラス | メソッド名  |
+================+========================+=============+
| AcmeBlogBundle | BlogController         | showAction  |
+----------------+------------------------+-------------+

コントローラーのコードは次のようになります。

.. code-block:: php

    // src/Acme/BlogBundle/Controller/BlogController.php
    namespace Acme\BlogBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    
    class BlogController extends Controller
    {
        public function showAction($slug)
        {
            // ...
        }
    }

コントローラークラスのクラス名に\ ``Controller`` を追加する必要があることに注意してください。\
この例では、コントローラー名 ``Blog`` は ``BlogController`` クラスに対応します。\
同様に、メソッド名には ``Action`` を追加する必要があります(``show`` => ``showAction``)。

ルートの論理コントローラーには、\ ``Acme\BlogBundle\Controller\BlogController::showAction`` のように、フルパスのクラス名、およびメソッド名で指定することも可能です。\
論理名で指定するほうが簡単ですし、より柔軟性があります。

.. note::

   ルートのコントローラーは、論理名による指定、フルパスによる指定の他、3 つ目の指定方法があります。\
   コロンを 1 つだけ使い、サービスのメソッドをコントローラーに指定できます (例: ``service_name:indexAction``)。\
   詳細は :doc:`/cookbook/controller/service` を参照してください。

パラメーターとコントローラーの引数
----------------------------------

``{slug}`` のようなパラメータは、コントローラーメソッドの引数として渡されるため、\
特に重要です。

.. code-block:: php

    public function showAction($slug)
    {
      // ...
    }

実際には、これらのパラメータ値は、\ ``defaults`` と一緒に1つの配列にマージされます。\
この配列の各キーは、コントローラーの引数として使用されることになります。

つまり、Symfony は、コントローラーメソッドの各引数に対して、\
その名前のパラメータをルートから探して、見つかった値をその引数にアサインします。\
先程の複雑な例では、\ ``showAction()`` メソッドの引数として、次のような変数が、\
どんな組み合わせでも、どんな順番でも使用可能になります。

* ``$culture``
* ``$year``
* ``$title``
* ``$_format``
* ``$_controller``

プレースホルダと ``defaults`` がマージされるので、変数 ``$_controller`` も使用可能になります。\
より詳しい情報は、 :ref:`route-parameters-controller-arguments` を参照してください。

.. tip::

    変数 ``$_route`` も使用可能です。この変数には、マッチしたルートの名前がセットされます。

.. index::
   single: Routing; Importing routing resources

.. _routing-include-external-resources:

外部のルーティングリソースをインクルード
----------------------------------------

すべてのルートは、ただひとつの設定ファイル、通常は ``app/config/routing.yml`` を通してロードされます(上述の `ルートを定義する`_ を参照)。\
とはいえ、その他の場所からもロードしたい場合が多々あるでしょう。\
たとえば、バンドル内部のルーティングファイルを読み込みたい場合など。\
こういう場合には、そのファイルを「インポート」することができます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        acme_hello:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection(
            $loader->import("@AcmeHelloBundle/Resources/config/routing.php")
        );

        return $collection;

.. note::

   YAML からリソースをインポートする場合は、そのキー名(上の例だと ``acme_hello``)は意味がなくなります。\
   他の行が上書きしないように、ユニークな名前にしておけば問題ありません。

キー ``resource`` で、与えられたルーティングリソースを読み込みます。\
この例では、resource はファイルへのフルパスです(ショートカット ``@AcmeHelloBundle`` によりバンドルのパスが解決される)。\
インポートされる側のファイルは次のようになります。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/routing.yml
       acme_hello:
            path:     /hello/{name}
            defaults: { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="acme_hello" path="/hello/{name}">
                <default key="_controller">AcmeHelloBundle:Hello:index</default>
            </route>
        </routes>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('acme_hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

        return $collection;

このファイル内のルートは、メインのルーティングファイルと同様にパースされロードされます。

インポートしたルートにプリフィクスを付ける
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

また、インポートされたルートに対して、"prefix" を与えることも可能です。\
例えば、ルート ``acme_hello`` が、最終的には、 ``/hello/{name}`` ではなくて、\
``/admin/hello/{name}`` というパスになっていたいとしましょう。

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        acme_hello:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"
            prefix:   /admin

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml"
                prefix="/admin" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();

        $acmeHello = $loader->import(
            "@AcmeHelloBundle/Resources/config/routing.php"
        );
        $acmeHello->setPrefix('/admin');

        $collection->addCollection($acmeHello);

        return $collection;

ロードされたルーティングリソースの各ルートパスに、\ ``/admin`` という文字がプリフィックスとして追加されます。

.. tip::

    コントローラーのメソッドにアノテーションを使ってルートを定義することもできます。
    指定方法についての詳細は :doc:`FrameworkExtraBundle documentation </bundles/SensioFrameworkExtraBundle/annotations/routing>` を参照してください。

インポートしたルートに requirements のホスト要件を組み込むには
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

インポートした一連のルートに、正規表現によるホストの振り分けを追加できます。
詳細は :ref:`component-routing-host-imported` を参照してください。

.. index::
   single: Routing; Debugging

ルートの可視化とデバッグ
------------------------

ルートを追加したりカスタマイズしているときに、ルート情報が可視化され、\
詳細な情報を得ることが出来れば便利です。\
アプリケーション内のすべてのルートの情報を見るには、コンソールコマンドの ``router:debug`` がいい方法です。\
プロジェクトルートで次のコマンドを実行します。

.. code-block:: bash

    $ php app/console router:debug

設定した\ *全て*\ のルートに対する、有益なリストが出力されます。

.. code-block:: text

    homepage              ANY       /
    contact               GET       /contact
    contact_process       POST      /contact
    article_show          ANY       /articles/{culture}/{year}/{title}.{_format}
    blog                  ANY       /blog/{page}
    blog_show             ANY       /blog/{slug}

あるルートのより詳細な情報をみるには、そのルート名を追加します。

.. code-block:: bash

    php app/console router:debug article_show

設定したルートが URL にマッチするかどうか手軽にテストしたい場合は、\ ``router:match`` コンソールコマンドを使います。

.. code-block:: bash

    $ php app/console router:match /blog/my-latest-post

URL にマッチしたルートが表示されます。

.. code-block:: text

    Route "blog_show" matches

.. index::
   single: Routing; Generating URLs

URL の生成
----------

ルーティングシステムは、URL を生成するためにも使用すべきです。\
ルーティングシステムは、\
URL とコントローラー/パラメーターをマッピングし、\
ルート/パラメーターから URL を生成する、双方向のシステムになっています。\
:method:`Symfony\\Component\\Routing\\Router::match` および、\
:method:`Symfony\\Component\\Routing\\Router::generate` がそれに当たります。\
先ほどの ``blog_show`` の例では、次のようになります。

.. code-block:: php

    $params = $this->get('router')->match('/blog/my-blog-post');
    // array(
    //     'slug'        => 'my-blog-post',
    //     '_controller' => 'AcmeBlogBundle:Blog:show',
    // )

    $uri = $this->get('router')->generate('blog_show', array('slug' => 'my-blog-post'));
    // /blog/my-blog-post

ルートを使って URL を生成するには、ルート名 (``blog_show``) と、そのルートのパスで使用されている\
すべてのワイルドカードの値 (``slug = my-blog-post``) を指定する必要があります。
この情報を元に、URL が生成されます。

.. code-block:: php

    class MainController extends Controller
    {
        public function showAction($slug)
        {
            // ...

            $url = $this->generateUrl(
                'blog_show',
                array('slug' => 'my-blog-post')
            );
        }
    }

.. note::

    上の例のように、Symfony の基底
    :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller` を継承したコントローラーでは、ルートの生成に :method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::generateUrl` メソッドを利用できます。
    このメソッドは、内部で router サービスの :method:`Symfony\\Component\\Routing\\Router::generate` メソッドを内部的に呼び出しています。

次節では、テンプレート内から URL を生成する方法を見ていきます。

.. tip::

    アプリケーションのフロントエンドで Ajax リクエストを使っている場合、JavaScript 内でもルーティングコンフィギュレーションに基づいた URL 生成を行いたいでしょう。
    この要な場合、\ `FOSJsRoutingBundle`_ バンドルを使います。

    .. code-block:: javascript

        var url = Routing.generate(
            'blog_show',
            {"slug": 'my-blog-post'}
        );

    詳細は、バンドルに付属するドキュメントを参照してください。

.. index::
   single: Routing; Absolute URLs

絶対 URL の生成
~~~~~~~~~~~~~~~

デフォルトでは、ルーターは相対 URL を生成します (``/blog``) 。\
絶対 URL を生成する場合は、\ ``generate()`` メソッドの第 3 引数に ``true`` を渡します。

.. code-block:: php

    $this->get('router')->generate('blog_show', array('slug' => 'my-blog-post'), true);
    // http://www.example.com/blog/my-blog-post

.. note::

    絶対 URL のホスト部分には、現在の ``Request`` オブジェクトのホストが使用されます。\
    ホスト情報は PHP のサーバー情報から自動的に検出されるため、\
    コマンドラインから実行するスクリプトの場合は、\ ``RequestContext`` オブジェクトで明示的にホストを指定してください。
    
    .. code-block:: php
    
        $this->get('router')->getContext()->setHost('www.example.com');

.. index::
   single: Routing; Generating URLs in a template

クエリーストリング付き URL
~~~~~~~~~~~~~~~~~~~~~~~~~~

``generate`` メソッドは、ワイルドカードの配列を引数に取りますが、\
ルートパラメーターに存在しないキーを渡すと、クエリーストリングとして URI に付加されます。

.. code-block:: php

    $this->get('router')->generate('blog', array('page' => 2, 'category' => 'Symfony'));
    // /blog/2?category=Symfony

テンプレートにおける URL 生成
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

アプリケーションでページ間をリンクするために、テンプレートで URL を生成したい場合もあります。\
この場合は、前述のようにもできますが、テンプレートヘルパー関数を使用してもできます。

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('blog_show', {'slug': 'my-blog-post'}) }}">
            記事を読む
        </a>

    .. code-block:: html+php

        <a href="<?php echo $view['router']->generate('blog_show', array(
            'slug' => 'my-blog-post',
        )) ?>">
            記事を読む
        </a>

絶対 URL も生成可能です。

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ url('blog_show', {'slug': 'my-blog-post'}) }}">
            記事を読む
        </a>

    .. code-block:: html+php

        <a href="<?php echo $view['router']->generate('blog_show', array(
            'slug' => 'my-blog-post',
        ), true) ?>">
            記事を読む
        </a>

まとめ
------

ルーティングは、リクエストされた URL を、そのリクエストが処理されるべきコントローラーに\
マッピングするシステムです。\
美しい URL を指定すること、アプリケーションの機能とその URL を疎にしておくことが可能です。\
また、ルーティングは 2 方向のメカニズムで、URL を生成する場合にも使用されます。

クックブックでより深く
----------------------

* :doc:`/cookbook/routing/scheme`

.. _`FOSJsRoutingBundle`: https://github.com/FriendsOfSymfony/FOSJsRoutingBundle

.. 2011/08/01 gilbite 59204716e7cee39acecfd140d231308f05f71e7d
.. 2014/01/01 hidenorigoto 14b0c47d20ceec01a3eaf8f8d37ca4f834ecf8d1
