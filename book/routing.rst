.. index::
   single: Routing

ルーティング
============

まじめな WEB アプリケーションであれば、美しい URL というのは絶対に必要になります。\
``index.php?article_id=57`` のような醜い URL は捨て去って、\
``/read/intro-to-symfony`` のような URL にしましょう、ということです。

加えて、柔軟性を得られるということがもっと重要な点です。\
例えば、とあるページの URL を、\ ``/blog`` から ``/news`` に変更する場合はどうしたらよいでしょうか？\
どれだけのリンクを探し出して、そして変更する必要があるでしょうか。\
Symfony のルータを使えば、変更は簡単です。

Symfony2 ルータでは、クリエティブな URL をいくつも定義できて、それらを、アプリケーション内の異なる場所にマップすることができます。\
この章が終わる頃には、次のようなことが可能になっているでしょう。

* 複雑なルートで、コントローラにマッピングすること
* テンプレートやコントローラ内で URL を生成すること
* バンドル(もしくはそれ以外でも)から、ルーティングリソースをロードすること
* ルートをデバッグすること

.. index::
   single: Routing; Basics

Routing in Action
-----------------

*ルート* の意味するところは、URL のパターンとコントローラのマップです。\
例えば、\ ``/blog/my-post`` や ``/blog/all-about-symfony`` のような URL にマッチしたときに、\
コントローラに送り、該当するブログエントリを探してレンダリングさせたいとしましょう。\
このルートはシンプルです。


.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        blog_show:
            pattern:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" pattern="/blog/{slug}">
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

ルート ``blog_show`` の pattern は、\ ``/blog/*`` のような URL に該当し、\
ワイルドカード部分には、\ ``slug`` という名前が与えられています。\
``/blog/my-blog-post`` という URL の場合は、変数 ``slug`` が ``my-blog-post`` という\
値となり、コントローラ上でも使用することができます(後述します)。

パラメータ ``_controller`` は、URL がマッチした際に実行されるべきコントローラを Symfony に伝える特別なキーです。\
``_controller`` 欄の文字列は、\ :ref:`論理名<controller-string-syntax>`\ と呼び、\
あるパターンに従って、特定の PHP クラスやメソッドを指定します。

.. code-block:: php

    // src/Acme/BlogBundle/Controller/BlogController.php
    
    namespace Acme\BlogBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class BlogController extends Controller
    {
        public function showAction($slug)
        {
            $blog = // 変数 $slug を使用してデータベースに問い合わせ
            
            return $this->render('AcmeBlogBundle:Blog:show.html.twig', array(
                'blog' => $blog,
            ));
        }
    }

おめでとう！これで、ルートがひとつできて、コントローラにもつながりました。\
``/blog/my-post`` に誰かがアクセスしたら、 ``showAction`` コントローラが実行され、\
変数 ``$slug`` は ``my-post`` を意味するようになります。

.. todo brushup

Symfony2 ルータの目標は、リクエストの URL をコントローラにマップすることです。\
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
どのコントローラを実行するべきか決定することです。\
このプロセスは下記のような流れになります。

#. Symfony2 フロントコントローラ(\ ``app.php``)によって、リクエストが処理される

#. Symfony2 のコア(Kernel) がルータにリクエストを調べるよう頼む

#. ルータは、URL とルートをマッチさせ、マッチしたルートの情報を返す。
   この情報には、どのコントローラが実行されるべきか、という情報が含まれている

#. Kernel が、特定されたコントローラを実行する。コントローラは最終的には ``Response`` オブジェクトを返す

.. figure:: /images/request-flow.png
   :align: center
   :alt: Symfony2 リクエストフロー

   ルーティング層は、URL を、実行すべきコントローラに変換するツール

.. index::
   single: Routing; Creating routes

ルートを作る
------------

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
        <framework:config ...>
            <!-- ... -->
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'router'        => array('resource' => '%kernel.root_dir%/config/routing.php'),
        ));

.. tip::

    すべてのルートが単一のファイルからロードされるとはいっても、\
    よくあるプラクティスとして、そのファイルから別のリソースをインクルードするというものがあります。\
    詳細は、\ :ref:`routing-include-external-resources` 節を参照してください。

基本的なルートの設定
~~~~~~~~~~~~~~~~~~~~

ルートの定義は簡単ですが、典型的なアプリケーションでは多数のルートをもつことになるでしょう。\
ルートは、基本的には2つのパート、すなわち、マッチさせるための ``pattern`` と ``defaults`` 配列からなります。

.. configuration-block::

    .. code-block:: yaml

        _welcome:
            pattern:   /
            defaults:  { _controller: AcmeDemoBundle:Main:homepage }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="_welcome" pattern="/">
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

このルートは、トップページ(``/``) にマッチし、\ ``AcmeDemoBundle:Main:homepage`` コントローラにマップされています。\
``_controller`` 部分の文字列は、Symfony2 によって実際の PHP 的関数に変換されて、実行されます。\
この動きに関しては、 :ref:`controller-string-syntax` 節で説明します。


.. index::
   single: Routing; Placeholders

プレースホルダ付きのルート
~~~~~~~~~~~~~~~~~~~~~~~~~~

もちろん、もっと面白いルートもサポートしています。\
多くのルートでは、「ワイルドカード」と呼ばれるプレースホルダを、1つ以上含むことになるでしょう。

.. configuration-block::

    .. code-block:: yaml

        blog_show:
            pattern:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" pattern="/blog/{slug}">
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

この pattern では、\ ``/blog/*`` のような URL には全てマッチします。\
加えて、コントローラ内では、プレースホルダ ``{slug}`` にマッチした値が使用できます。\
``/blog/hello-world`` という URL の場合だと、変数 ``$slug`` には ``hello-world`` という値が割り当てられ、\
コントローラ内で使用可能となります。\
たとえば、その値を利用してブログエントリを読み込む事が可能になります。

ただし、この pattern では、単に ``/blog`` という ULR にはマッチ\ *しません*\ 。\
なぜなら、全てのプレースホルダはデフォルトでは必須となっているからです。\
これを避けるには、\ ``defaults`` 配列にプレースホルダの値を与えておくとよいでしょう。

プレースホルダの必須/任意設定
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

話をおもしろくするために、この仮ブログアプリケーションに、全エントリのリストを表示するルートを追加してみましょう。

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog
            defaults:  { _controller: AcmeBlogBundle:Blog:index }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog">
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
プレースホルダも入っていないので、\ ``/blog`` という URL にだけマッチすることになります。\
それでは、このルートが、ページネーションをサポートしたいとしたらどうでしょう。\
``/blog/2`` で2ページ目を表示させたいとします。\
\ ``{page}`` プレースホルダを足してみましょう。

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
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

先程の\ ``{slug}`` と同様に、\ ``{page}`` 部分にマッチした値は、コントローラ内で使うことができます。\
そのページで、どのブログエントリ群を表示するかを決定するために使用すればいいのです。

ちょっと待って下さい！\
プレースホルダはデフォルトでは必須なので、このルートでは、もはや単純な ``/blog`` にはマッチしなくなってしまいます。\
1ページ目を見たければ、代わりに、\ ``/blog/1`` という URL にする必要があります！\
とはいえ、これはリッチな WEB アプリではあり得ない話なので、\
``{page}`` パラメータは任意としましょう。\
任意にするには、\ ``defaults`` に追加します。

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
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
            'page' => 1,
        )));

        return $collection;

``defaults`` に ``page`` を追加すると、\ ``{page}`` は必須ではなくなります。\
こうしておくと、このルートにURL ``/blog`` がマッチするようになります。\
その場合、\ ``page`` パラメータは ``1`` にセットされます。\
もちろん ``/blog/2`` にもマッチし、その場合の ``page`` は ``2`` になります。\
完璧。

+---------+------------+
| /blog   | {page} = 1 |
+---------+------------+
| /blog/1 | {page} = 1 |
+---------+------------+
| /blog/2 | {page} = 2 |
+---------+------------+

.. index::
   single: Routing; Requirements

requirements をつける
~~~~~~~~~~~~~~~~~~~~~

今までのルートをまとめて見てみましょう。

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }

        blog_show:
            pattern:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
            </route>

            <route id="blog_show" pattern="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page' => 1,
        )));

        $collection->add('blog_show', new Route('/blog/{show}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

これでは問題があるのがわかりますか？\
両方のルートが ``/blog/*`` のような URL をマッチさせてしまう pattern になっています。\
Symfony ルータでは、常に\ **最初**\ にマッチしたルートが適用されます。\
従って、この例では ``blog_show`` ルートには\ *絶対に*\ マッチしません。\
``/blog/my-blog-post`` のような URL でも、最初のルート(``blog``)にマッチしてしまい、\
``{page}`` には意味をなさない ``my-blog-post`` という値が返されてしまいます。

+--------------------+-------+-----------------------+
| URL                | route | parameters            |
+====================+=======+=======================+
| /blog/2            | blog  | {page} = 2            |
+--------------------+-------+-----------------------+
| /blog/my-blog-post | blog  | {page} = my-blog-post |
+--------------------+-------+-----------------------+

この問題の解は、\ *requirements* を追加することです。\
今回の例では、pattern である ``/blog/{page}`` には、\ ``{page}`` 部分が数字である URL に\ *のみ*\ マッチすればうまくいくはずです。\
ありがたい事に、各パラメータには、簡単に正規表現の requirements を追加していくことができます。

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }
            requirements:
                page:  \d+

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
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
            'page' => 1,
        ), array(
            'page' => '\d+',
        )));

        return $collection;

``\d+`` の部分は、パラメータ ``{page}`` が数字でなければならない、という正規表現です。\
ルート ``blog`` は、\ ``/blog/2`` のような URL にはマッチしますが(2は数字だから)、\
``/blog/my-blog-post`` のような URL にはもうマッチしません(``my-blog-post`` は\ *数字ではない*\ から)。

こうして、\ ``/blog/my-blog-post`` のような URL が、正しくルート ``blog_show`` にマッチすることになります。

+--------------------+-----------+-----------------------+
| URL                | route     | parameters            |
+====================+===========+=======================+
| /blog/2            | blog      | {page} = 2            |
+--------------------+-----------+-----------------------+
| /blog/my-blog-post | blog_show | {slug} = my-blog-post |
+--------------------+-----------+-----------------------+

.. sidebar:: 最初が勝つ

    これの意味することは、ルートの順番がとても重要だということです。\
    ルート ``blog_show`` が、ルート ``blog`` より上に位置していれば、\
    ``/blog/2`` は、\ ``blog`` ではなく、\ ``blog_show`` にマッチするでしょう。\
    どうしてかというと、\ ``blog_show`` の ``{slug}``  には、\
    requirements が設定されていないからです。\
    正しい順番で、うまく requirements を使用していくことで、大概の事は可能になります。

requirements は正規表現なので、複雑さ、柔軟さという点では、完全にあなた次第です。\
それでは、URL に応じて二つの言語が有効なアプリケーションを考えてみましょう。

.. configuration-block::

    .. code-block:: yaml

        homepage:
            pattern:   /{culture}
            defaults:  { _controller: AcmeDemoBundle:Main:homepage, culture: en }
            requirements:
                culture:  en|fr

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="homepage" pattern="/{culture}">
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
            'culture' => 'en',
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

HTTP メソッド の requirements をつける
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

マッチさせる部分は、URL に加えて、\ *メソッド*\ (GET, HEAD, POST, PUT, DELETE)があります。\
2つのコントローラからなるお問い合わせフォームを考えてみましょう。\
1つは、GET リクエストでフォームを表示するコントローラ、\
もう1つは、POST リクエストで送信されたフォームを処理するコントローラです。\
次のようなルート設定で、これが実現可能になります。

.. configuration-block::

    .. code-block:: yaml

        contact:
            pattern:  /contact
            defaults: { _controller: AcmeDemoBundle:Main:contact }
            requirements:
                _method:  GET

        contact_process:
            pattern:  /contact
            defaults: { _controller: AcmeDemoBundle:Main:contactProcess }
            requirements:
                _method:  POST

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="contact" pattern="/contact">
                <default key="_controller">AcmeDemoBundle:Main:contact</default>
                <requirement key="_method">GET</requirement>
            </route>

            <route id="contact_process" pattern="/contact">
                <default key="_controller">AcmeDemoBundle:Main:contactProcess</default>
                <requirement key="_method">POST</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contact',
        ), array(
            '_method' => 'GET',
        )));

        $collection->add('contact_process', new Route('/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contactProcess',
        ), array(
            '_method' => 'POST',
        )));

        return $collection;

この2つのルートは全く同じ pattern(``/contact``)となってはいるのですが、\
最初のルートは、GET リクエストにのみマッチするのに対して、\
二つ目のルートは、POST リクエストにのみマッチします。\
これの意味するところは、同一の URL でフォームと送信を表示することが可能で、かつ、\
異なる2つのコントローラを使い分けることが可能になる、ということです。

.. note::
    ``_method`` が指定されていない場合は、\ *すべて*\ のメソッドがマッチします。

他の requirements と同様に、 ``_method`` も正規表現としてパースされます。\
``GET`` *もしくは* ``POST`` リクエストにマッチさせたい場合は、\ ``GET|POST`` とします。

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
          pattern:  /articles/{culture}/{year}/{title}.{_format}
          defaults: { _controller: AcmeDemoBundle:Article:show, _format: html }
          requirements:
              culture:  en|fr
              _format:  html|rss
              year:     \d+

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="article_show" pattern="/articles/{culture}/{year}/{title}.{_format}">
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
        $collection->add('homepage', new Route('/articles/{culture}/{year}/{title}.{_format}', array(
            '_controller' => 'AcmeDemoBundle:Article:show',
            '_format' => 'html',
        ), array(
            'culture' => 'en|fr',
            '_format' => 'html|rss',
            'year' => '\d+',
        )));

        return $collection;

ご覧のとおり、このルートでは、URL の ``{culture}`` の部分が ``en`` か ``fr`` の場合で、\
かつ、\ ``{year}`` が数字の場合にのみマッチします。\
また、プレースホルダ間で、スラッシュとは別に、ピリオドを使用する方法も示しています。\
このルートでは下記のような URL がマッチします。

 * ``/articles/en/2010/my-post``
 * ``/articles/fr/2010/my-post.rss``

.. sidebar:: ルーティングパラメータ ``_format``

    この例では、ルーティングパラメータ ``_format`` も目玉部分です。\
    このパラメータを使うと、マッチした値は ``Request`` オブジェクトの\
    「リクエストフォーマット」になります。\
    最終的には、このリクエストフォーマットは、レスポンスの ``Content-Type`` 設定に使用されたりします。\
    例えば、\ ``json`` は、``Content-Type`` では ``application/json`` に変換されます。\
    また、コントローラ内で、 ``_format`` の値に応じて、異なるテンプレートを出力するためにも使用されます。\
    ``_format`` パラメータは、同一のコンテンツを異なるフォーマットで出力する、\
    とても強力な方法です。

.. index::
   single: Routing; Controllers
   single: Controller; String naming format

.. _controller-string-syntax:

コントローラの指定パターン
--------------------------

すべてのルートは、パラメータ ``_controller`` を持っている必要があります。\
ルートがマッチした際に、このパラメータによって、どのコントローラが実行されるかが決定されます。\
この値のことを\ *論理コントローラ名*\ と呼んでいますが、シンプルな文字列パターンに従っており、\
Symfony が特定の PHP メソッド・クラスにマッピングする際に使用されます。\
3つの部分からなり、それぞれコロンで区分けされます。


    **バンドル**:**コントローラ**:**アクション**


たとえば、\ ``_controller`` の値が ``AcmeBlogBundle:Blog:show`` となっている場合は、下記のような意味合いになります。

+----------------+----------------------+-------------+
| バンドル       | コントローラのクラス | メソッド名  |
+================+======================+=============+
| AcmeBlogBundle | BlogController       | showAction  |
+----------------+----------------------+-------------+

コントローラは次のようになります。

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

Symfony が、クラス名に\ ``Controller`` という文字列を付加していることに注意してください。\
この例では、\ ``Blog`` は ``BlogController`` となります。\
同様に、メソッド名には、\ ``Action`` が付加されます(``show`` => ``showAction``)。

コントローラは、\ ``Acme\BlogBundle\Controller\BlogController::showAction`` のように、フルパスのクラス名、及びメソッド名でも指定可能です。\
ただし、シンプルに行こうということなら、論理名で指定するほうが簡単ですし、より柔軟性があります。

.. note::

   論理名、フルパス指定に加えて、Symfony には3つ目の指定方法があります。\
   コロンを1つだけ使用する方法です(例: ``service_name:indexAction``)。\
   コントローラをサービスとして使用する方法です。\
   詳細は :doc:`/cookbook/controller/service` を参照してください。

パラメータとコントローラの引数
------------------------------

``{slug}`` のようなパラメータは、コントローラメソッドの引数として渡されるため、\
特に重要です。

.. code-block:: php

    public function showAction($slug)
    {
      // ...
    }

.. todo brushup

実際には、これらのパラメータ値は、\ ``defaults`` と一緒に1つの配列にマージされます。\
この配列の各キーは、コントローラの引数として使用されることになります。

つまり、Symfony は、コントローラメソッドの各引数に対して、\
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

すべてのルートは、ただひとつの設定ファイル、通常は ``app/config/routing.yml`` を通してロードされます(上述の `ルートを作る`_ を参照)。\
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
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@AcmeHelloBundle/Resources/config/routing.php"));

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
            pattern:  /hello/{name}
            defaults: { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="acme_hello" pattern="/hello/{name}">
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
``/admin/hello/{name}`` というふうになっていたいとしましょう。

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
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" prefix="/admin" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@AcmeHelloBundle/Resources/config/routing.php"), '/admin');

        return $collection;

ロードされたルーティングリソースの各ルート pattern に、\ ``/admin`` という文字がプリフィックスとして追加されます。

.. index::
   single: Routing; Debugging

ルートの可視化とデバッグ
------------------------

ルートを追加したりカスタマイズしているときに、ルート情報が可視化され、\
詳細な情報を得ることが出来れば便利です。\
アプリケーション内のすべてのルートの情報を見るには、コンソールコマンドの ``router:debug`` がいい方法です。\
プロジェクトルートで下記のコマンドを実行します。

.. code-block:: bash

    php app/console router:debug

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

.. index::
   single: Routing; Generating URLs

URL の生成
----------

ルーティングシステムは、URL を生成するためにも使用すべきです。\
ルーティングシステムは、\
URL とコントローラ/パラメータをマッピングし、\
そして、ルート/パラメータから URL を生成する、双方向のシステムになっています。\
:method:`Symfony\\Component\\Routing\\Router::match` 及び、\
:method:`Symfony\\Component\\Routing\\Router::generate` がそれに当たります。\
先の ``blog_show`` の例では、下記のようになります。::

    $params = $router->match('/blog/my-blog-post');
    // array('slug' => 'my-blog-post', '_controller' => 'AcmeBlogBundle:Blog:show')

    $uri = $router->generate('blog_show', array('slug' => 'my-blog-post'));
    // /blog/my-blog-post

URL を生成するには、ルート名(``blog_show``)と、そのルートのパターンで使用されている\
すべてのワイルドカード(``slug = my-blog-post``)を指定する必要があります。
この情報を元に、URL が簡単に生成されます。

.. code-block:: php

    class MainController extends Controller
    {
        public function showAction($slug)
        {
          // ...

          $url = $this->get('router')->generate('blog_show', array('slug' => 'my-blog-post'));
        }
    }

次節では、テンプレート内から URL を生成する方法を見ていきます。

.. index::
   single: Routing; Absolute URLs

絶対 URL の生成
~~~~~~~~~~~~~~~

デフォルトでは、ルータは相対 URL を生成します(``/blog``)。\
絶対 URL の場合は、\ ``generate()`` メソッドの第三引数に、 ``true`` を渡します。

.. code-block:: php

    $router->generate('blog_show', array('slug' => 'my-blog-post'), true);
    // http://www.example.com/blog/my-blog-post

.. note::

    絶対 URL のホスト部分は、現在の ``Request`` オブジェクトのホストが使用されます。\
    ホスト情報は PHP のサーバ情報から自動的に検出されるため、\
    コマンドラインから実行するスクリプトの場合は、\
    希望するホストを、手動で ``Request`` オブジェクトに設定してやる必要があります。
    
    .. code-block:: php
    
        $request->headers->set('HOST', 'www.example.com');

.. index::
   single: Routing; Generating URLs in a template

クエリストリング付き URL
~~~~~~~~~~~~~~~~~~~~~~~~

``generate`` メソッドは、ワイルドカードの配列を引数に取りますが、\
そこにそれ以外の値を渡すと、クエリストリングとして URI に付加されます。:: 

    $router->generate('blog', array('page' => 2, 'category' => 'Symfony'));
    // /blog/2?category=Symfony

テンプレートで URL 生成
~~~~~~~~~~~~~~~~~~~~~~~

URL を生成する場として、一番ありがちなのが、アプリケーション内のリンクを貼るテンプレート内でしょう。\
この場合は、前述のようにもできますが、テンプレートヘルパ関数を使用してもできます。

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('blog_show', { 'slug': 'my-blog-post' }) }}">
          Read this blog post.
        </a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('blog_show', array('slug' => 'my-blog-post')) ?>">
            Read this blog post.
        </a>

絶対 URL も生成可能です。

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ url('blog_show', { 'slug': 'my-blog-post' }) }}">
          Read this blog post.
        </a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('blog_show', array('slug' => 'my-blog-post'), true) ?>">
            Read this blog post.
        </a>

まとめ
------

ルーティングは、リクエストされたURLを、そのリクエストが処理されるべきコントローラ関数に\
マッピングするシステムです。\
美しい URL を指定すること、アプリケーションの機能とその URL を疎にしておくことが可能です。\
また、ルーティングは2方向のメカニズムで、URL を生成する場合にも使用されます。

クックブックでより深く
----------------------

* :doc:`/cookbook/routing/scheme`


.. 2011/08/01 gilbite 59204716e7cee39acecfd140d231308f05f71e7d
