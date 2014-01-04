.. index::
   single: Page creation

.. note::

    * 対象バージョン：2.4 (2.1以降)
    * 翻訳更新日：2014/01/04

Symfony2 でのページ作成
=======================

Symfony2 における新しいページの作成は、次の 2 つの簡単なステップから構成されます。

* *ルート(route)の作成*: ルートは作成するページへの URL\ (例えば ``about``\ )を定義し、\
  Symfony2 が実行すべきコントローラー(PHP 関数)を特定します。\
  アプリケーションが受け取るリクエストの URL がルートのパスにマッチするときに使われます。

* *コントローラーの作成*: コントローラーは、リクエストを受け取り、\
  何らかの処理を行った後、ユーザーに返す ``Response`` オブジェクトに変換するような PHP 関数です。

このシンプルな方法は、Web の仕組みと一致していてとても美しいと言えます。\
Web 上のあらゆる相互通信は HTTP リクエストによって開始されます。\
アプリケーションの動作は、リクエストを読み取って適切な HTTP レスポンスを返すだけです。

Symfony2 はこの哲学に従い、トラフィックや複雑さが膨れ上がるようなアプリケーションをうまく整理するためのツールや規約を提供しています。

.. index::
   single: Page creation; Environments & Front Controllers

.. _page-creation-environments:

環境とフロントコントローラー
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony アプリケーションには特定の :term:`環境` に結び付けられて実行されます。\
環境は何らかの文字列で表され、コンフィギュレーションと読み込むバンドルのセットを指します。\
1 つのアプリケーションを、複数の環境で実行できます。\
Symfony2 にはデフォルトで 3 つの環境が用意されています
— ``dev``\ 、\ ``test``\ 、\ ``prod`` — 独自の環境を定義することもできます。

環境を使うと、1 つのアプリケーションに対してデバッグ等が有効になった dev 環境と速度が最適化された prod 環境を持つことができます。\
特定の環境でのみ読み込むバンドルを指定することもできます。\
たとえば、Symfony2 の WebProfilerBundle (以降で説明) は、\ ``dev`` と ``test`` 環境でのみ読み込まれます。

また、Symfony2 には Web 用に 2 つのフロントコントローラーが用意されています: ``dev`` 環境の ``app_dev.php`` と、\
``prod`` 環境の ``app.php`` です。\
Symfony2 では、すべての Web アクセスは通常、このいずれかのフロントコントローラーで処理します。\
(``test`` 環境はユニットテストの実行時にのみ使われるので、Web フロントコントローラーはありません。\
コンソールツールにもフロントコントローラーがあり、任意の環境で使えます。)

フロントコントローラーにおける kernel の初期化時に、次の 2 つのパラメーターが渡されます:
環境と、kernel のデバッグモードフラグです。\
アプリケーションのレスポンスを速くするために、\ ``app/cache/`` ディレクトリにさまざまなキャッシュが作成されます。\
kernel のデバッグモードを有効にした場合 (``app_dev.php`` ではデフォルトで有効)、\
ソースコードやコンフィギュレーションに変更があると自動的にキャッシュがクリアされます。\
デバッグモードが有効な場合は、Symfony アプリケーションのレスポンスは遅くなってしまいますが、\
何か変更した場合に手作業でキャッシュをクリアする必要がなくなるので、開発時には便利です。

.. index::
   single: Page creation; Example

「Hello Symfony!」ページ
------------------------

クラシックな「Hello World!」アプリケーションからスタートしてみましょう。\
このアプリケーションでは、以下の URL にアクセスするとあいさつページ(例えば「Hello Symfony」)が返されるようにします。

.. code-block:: text

    http://localhost/app_dev.php/hello/Symfony

URL の ``Symfony`` 部分を他の名前に置き換えられるようにします。\
ページを作成するために、2 つの簡単な手順を踏んでください。

.. note::

    チュートリアルではすでに Symfony2 をダウンロードして Web サーバーが設定されていることを想定しています。\
    また、上の URL は、\ ``localhost`` のドキュメントルートが新しい Symfony2 プロジェクトの ``web`` ディレクトリに割り当てられている想定です。\
    この手順についての詳しい説明は、お使いの Web サーバーのドキュメントを参照してください。\
    Web サーバーごとのドキュメントのリンクは以下です。

    * Apache HTTP Server `Apache's DirectoryIndex documentation`_
    * Nginx `Nginx HttpCoreModule location documentation`_

ページを作り始める前に: バンドルの作成
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ページを作り始める前に、\ *バンドル* を作る必要があります。\
Symfony2 では、アプリケーション内のすべてのコードを\ :term:`バンドル`\ という構造の中に記述します。\
端的には、バンドルはプラグインのようなものと言えます。

バンドルとは、アプリケーションで実装する何らかの機能に関連するあらゆるものを格納する、単なるディレクトリ構造です。\
PHP のクラスや設定ファイル、スタイルシート、JavaScript のファイルなどをバンドル内に作成します(\ :ref:`page-creation-bundles` を参照)。

ここでは ``AcmeHelloBundle`` (この章で作る予定のお遊びバンドル) と呼ぶバンドルを作成するので、\
次のコマンドを実行し、画面の指示に従ってください(標準のオプションを使います)。

.. code-block:: bash

    $ php app/console generate:bundle --namespace=Acme/HelloBundle --format=yml

コマンドを実行すると、\ ``src/Acme/HelloBundle`` というパスにバンドルのディレクトリが作成されます。\
カーネルにバンドルを登録する処理が、\ ``app/AppKernel.php`` ファイルに自動的に追加されます。

.. code-block:: php

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            ...,
            new Acme\HelloBundle\AcmeHelloBundle(),
        );
        // ...

        return $bundles;
    }

バンドルがセットアップされたので、バンドルの中にアプリケーションを構築できるようになりました。

ステップ 1: ルートの作成
~~~~~~~~~~~~~~~~~~~~~~~~

標準では、\ Symfony2 アプリケーションのルーティング設定は ``app/config/routing.yml`` にあります。\
Symfony2 の他の設定と同様に、YAML ではなく XML や PHP 形式でもルートの設定を記述できます。

メインのルーティングファイルを見ると、\ ``AcmeHelloBundle`` を作ったときに Symfony がすでにエントリを追加しているのがわかるでしょう。

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        acme_hello:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"
            prefix:   /

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml"
                prefix="/" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->addCollection(
            $loader->import('@AcmeHelloBundle/Resources/config/routing.php'),
            '/'
        );

        return $collection;

この設定では、個々のルート定義を ``Resources/config/routing.yml`` から読み込みます。\
読み込まれるファイルは ``AcmeHelloBundle`` の中にあります。\
ルートの定義は直接 ``app/config/routing.yml`` に記述するか、またはアプリケーションの任意のファイルに記述してインポートすることができます。

バンドルから ``routing.yml`` ファイルがインポートされることを確認しました。\
これから作ろうとしているページの URL を定義する新しいルートを追加しましょう。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/routing.yml
        hello:
            path:     /hello/{name}
            defaults: { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="hello" path="/hello/{name}">
                <default key="_controller">AcmeHelloBundle:Hello:index</default>
            </route>
        </routes>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

        return $collection;

このルーティングは 2 つの基本的な項目から構成されています。\
1 つ目は ``path`` で、このルートがマッチする URL のことです。2 つ目は ``defaults`` 配列で、実行されるべきコントローラーを特定しています。\
パスの中のプレースホルダー記法(``{name}``)はワイルドカードです。\
``/hello/Ryan`` や ``/hello/Fabien`` や他の同様の URL がマッチすることを意味しています。\
``{name}`` プレースホルダーパラメーターはコントローラーに渡されるので、この値を使ってレスポンスを組み立てることができます。

.. note::

  ルーティングシステムにはアプリケーションの URL 構造を柔軟かつパワフルに作るための、さまざまな機能があります。\
  詳細は\ :doc:`ルーティングの章 </book/routing>`\ を参照してください。

ステップ2: コントローラーの作成
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

アプリケーションに ``/hello/Ryan`` のような URL が渡されると、\ ``hello`` ルートがマッチしてフレームワークが ``AcmeHelloBundle:Hello:index`` コントローラーを実行します。\
ページ作成手順の 2 つ目のステップは、このコントローラーを作成することです。

``AcmeHelloBundle:Hello:index`` はコントローラーの\ *論理*\ 名で、\ ``Acme\HelloBundle\Controller\HelloController`` クラスの ``indexAction`` メソッドにマッピングされます。\
``AcmeHelloBundle`` の中にこのファイルを作成することから始めましょう。

.. code-block:: php

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    class HelloController
    {
    }

実は、コントローラーは、あなたが作成して Symfony が実行するメソッドに過ぎません。\
コントローラーは、リクエストされたリソースを構築し準備し、それらの情報を使うところです。\
いくらかの高度な場合を除けば、コントローラーメソッドから返されるのは常に Symfony2 の ``Response`` オブジェクトです。

``hello`` ルートがマッチしたときに Symfony が実行する ``indexAction`` メソッドを作りましょう。

.. code-block:: php

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
        public function indexAction($name)
        {
            return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

コントローラーでは単純に ``Response`` オブジェクトを作成して return しています。\
``Response`` オブジェクトの最初の引数は、レスポンスに使われるコンテンツです (例として小さなHTMLページを想定しています)。

ルートとコントローラーを１つずつ作り、アプリケーションに 1 つのページができました。\
正しくセットアップされていれば、アプリケーションがあいさつを返してくれるでしょう:

.. code-block:: text

    http://localhost/app_dev.php/hello/Ryan

.. _book-page-creation-prod-cache-clear:

.. tip::

    次の URL にアクセスすると、"prod" :ref:`環境 <environments-summary>`\ でのアプリケーションの動作を確認できます。

    .. code-block:: text

        http://localhost/app.php/hello/Ryan

    エラーが表示される場合は、次のコマンドを実行してキャッシュをクリアしてみてください。

    .. code-block:: bash

        $ php app/console cache:clear --env=prod --no-debug

この例では作成していませんが、一般的には 3 つ目のステップとしてテンプレートの作成があります。

.. note::

   ページを作成するときにはコントローラーは、書いたコードのメインのエントリポイントになり、重要な構成要素でもあります。詳しくは :doc:`コントローラーの章 </book/controller>` を参照してください。

オプションのステップ3: テンプレートの作成
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

テンプレートは、\ HTML コードなどのプレゼンテーションを別のファイルに分けることができ、ページレイアウトの異なる部分で再利用できるようになります。\
コントローラーの中に HTML を書く代わりにテンプレートを描画します。

.. code-block:: php

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
            return $this->render(
                'AcmeHelloBundle:Hello:index.html.twig',
                array('name' => $name)
            );

            // render a PHP template instead
            // return $this->render(
            //     'AcmeHelloBundle:Hello:index.html.php',
            //     array('name' => $name)
            // );
        }
    }

.. note::

   :method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::render` メソッドを使うために、コントローラーは
   :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller` クラスを拡張する必要があります。
   このクラスは、コントローラーの中でよく使われる動作のショートカットを追加しています。
   上のサンプルでは実装済みで、4 行目に ``use`` 文を追加して、6 行目でクラスを拡張しています。

``render()`` メソッドは、\ ``Response`` オブジェクトを作成しますが、このオブジェクトは描画されたテンプレートの内容で満たされています。\
他のコントローラーと同様に、最終的には ``Response`` オブジェクトを返しています。

テンプレートの描画について、2 つの異なる例があることに注意してください。\
標準では Symfony2 は 2 つの異なるテンプレート言語をサポートしています。\
1 つはクラシックな PHP テンプレートで、もう 1 つは簡潔ですが強力な `Twig`_ テンプレートです。\
心配しないでください。同じプロジェクト内でどちらかあるいはどちらも自由に選べます。

このコントローラーは ``AcmeHelloBundle:Hello:index.html.twig`` テンプレートを描画しますが、次のような命名規則を使っています:

    **バンドル名**:**コントローラー名**:**テンプレート名**

これはテンプレートの\ *論理的な*\ 名前で、次のような規則を用いた物理パスとのマッピングです:

    **/path/to/BundleName**/Resources/views/**ControllerName**/**TemplateName**

今回の場合は ``AcmeHelloBundle`` がバンドル名、\ ``Hello`` がコントローラー名、そして ``index.html.twig`` がテンプレート名です。

.. configuration-block::

    .. code-block:: jinja
       :linenos:

        {# src/Acme/HelloBundle/Resources/views/Hello/index.html.twig #}
        {% extends '::base.html.twig' %}

        {% block body %}
            Hello {{ name }}!
        {% endblock %}

    .. code-block:: html+php

        <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
        <?php $view->extend('::base.html.php') ?>

        Hello <?php echo $view->escape($name) ?>!

Twig テンプレートを１行１行見ていきましょう。

* *2 行目*: ``extends`` トークンは親のテンプレートを定義します。\
  親のテンプレートでは明示的にレイアウトファイルがどこに置かれるかを定義しています。

* *4 行目*: ``block`` トークンは ``body`` という名前のブロックの中に挿入されるものを示しています。\
  ご覧のとおり、親のテンプレート(``base.html.twig``) は ``body`` という名前のブロックが最終的に描画されることに対して責任を負います。

親のテンプレートである ``::base.html.twig`` は、名前から **バンドル名** と **コントローラー名** が無くなっていて、先頭が二重コロン(``::``)になっています。\
これはテンプレートがバンドルの外に存在していて、\ ``app`` ディレクトリの中にあることを意味しています。

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title>{% block title %}Welcome!{% endblock %}</title>
                {% block stylesheets %}{% endblock %}
                <link rel="shortcut icon" href="{{ asset('favicon.ico') }}" />
            </head>
            <body>
                {% block body %}{% endblock %}
                {% block javascripts %}{% endblock %}
            </body>
        </html>

    .. code-block:: html+php

        <!-- app/Resources/views/base.html.php -->
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title><?php $view['slots']->output('title', 'Welcome!') ?></title>
                <?php $view['slots']->output('stylesheets') ?>
                <link rel="shortcut icon" href="<?php echo $view['assets']->getUrl('favicon.ico') ?>" />
            </head>
            <body>
                <?php $view['slots']->output('_content') ?>
                <?php $view['slots']->output('javascripts') ?>
            </body>
        </html>

ベースのテンプレートファイルは HTML レイアウトを定義し、\ ``index.html.twig`` テンプレート内で定義した ``body`` ブロックを秒しています。\
このテンプレートは ``title`` ブロックも描画していて、\ ``index.html.twig`` テンプレート内で定義することもできます。\
``title`` ブロックを子テンプレートでで定義しなければ初期値で「Welcome!」となります。

テンプレートはページのコンテンツを描画し整理するための強力な方法です。\
テンプレートは HTML マークアップから CSS コード、あるいはコントローラーが返したいあらゆるものを描画できます。

リクエストのライフサイクルにおいて、テンプレートエンジンは単なるオプションツールです。\
各コントローラーの最終目標を思い出すと ``Response`` オブジェクトを返却することです。\
テンプレートは ``Response`` オブジェクトのコンテンツを作成するための強力で、しかしオプションの、ツールです。

.. index::
   single: Directory Structure

ディレクトリ構造
----------------

ほんのいくつかの節を経たことで、 Symfony2 においてページを作り描画する作業の裏側にある哲学をもう理解できました。\
また Symfony2 のプロジェクトがどのように構造化され整理されているかも分かり始めてきたでしょう。\
この節の終わりまでには様々なファイルがどこにあり、どこに置き、なぜそこに置くのかがわかるでしょう。

あらゆることに柔軟に対応できるのですが、標準では Symfony の :term:`アプリケーション` は共通の基本的なディレクトリ構造を持っていて、この構造は推奨されています。

* ``app/``: アプリケーション設定を含むディレクトリ

* ``src/``: プロジェクトのすべての PHP コードはこのディレクトリの下に格納されます

* ``vendor/``: 慣例ではあらゆるベンダーライブラリはここに置かれます

* ``web/``: ここはウェブルートディレクトリで、公開してアクセス可能なファイルはここに含めます

.. _the-web-directory:

ウェブディレクトリ
~~~~~~~~~~~~~~~~~~

ウェブルートディレクトリは公開する静的なファイルすべてを置く場所です。\
画像やスタイルシート、そして JavaScript も含みます。\
また次のような\ :term:`フロントコントローラー`\ を置く場所でもあります:

.. code-block:: php

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

フロントコントローラーは (``app.php`` を例にすると) Symfony2 を使うときに実行される PHP ファイルで、アプリケーションを起動するために ``AppKernel`` クラスを使います。

.. tip::

    フロントコントローラーを持っているということは、典型的なフラットな PHP アプリケーションとは違い、より柔軟な URL に対応できることを意味しています。\
    フロントコントローラーを使うとき、URL は次のように書きます。

    .. code-block:: text

        http://localhost/app.php/hello/Ryan

    フロントコントローラーの ``app.php`` が実行され、"内部的な" URL である ``/hello/Ryan`` は、アプリケーションのルーティング設定にしたがって処理されます。\
    Apache の ``mod_rewrite`` ルールを使えば、URL に ``app.php`` が現れないようにできます。

    .. code-block:: text

        http://localhost/hello/Ryan

フロントコントローラーはすべてのリクエストの扱いにおいての重要なポイントではありますが、修正することはほとんどなく、開発中に意識する必要はありません。\
フロントコントローラーについていは `環境`_ 節で再び説明します。

アプリケーション (``app``) ディレクトリ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

フロントコントローラーで見たように、\ ``AppKernel`` クラスはアプリケーションのメインのエントリポイントで、すべての設定に責任を持ちます。\
``app/`` ディレクトリの中に格納されているような設定です。

``AppKernel`` クラスは 2 つのメソッドを実装しなければならず、これらは Symfony がアプリケーションについて知るために必要なすべての定義です。\
開発を始めるときはこれらのメソッドに心配をする必要さえありません。\
Symfony が実用的な標準設定をしてくれています。

* ``registerBundles()``: アプリケーションで実行する必要があるバンドルの配列を返します。
  (:ref:`page-creation-bundles` を参照)

* ``registerContainerConfiguration()``: メインアプリケーションのリソースファイルを読み込みます。
  (`アプリケーション設定`_\ の節を参照)

日常的な開発においては、\ ``app/config/`` ディレクトリの中の設定やルーティングファイルを編集するために ``app/`` ディレクトリをよく使うでしょう(`アプリケーション設定`_\ を参照)。\
また ``app/`` ディレクトリは、アプリケーションキャッシュディレクトリ(``app/cache``)やログディレクトリ(``app/logs``)、そしてテンプレート(``app/Resources``)などのアプリケーションレベルのリソースファイルなども含みます。\
これらのディレクトリについては後の章でより詳しく学べるでしょう。

.. _autoloading-introduction-sidebar:

.. sidebar:: 自動読み込み(オートローディング)

    Symfony がロードされるとき、\ ``vendor/autoload.php`` という特別なファイルが読み込まれます。\
    このファイルは Composer により作成され、\ ``src/`` ディレクトリからアプリケーションのファイルを、\ ``vendor/`` ディレクトリからサードパーティのライブラリをオートロードできるようにします。

    オートローダーがあるので、\ ``include`` や ``require`` を書くことに心配になる必要は全くありません。\
    その代わりに、クラスが必要になった時点で Composer がクラスの名前空間から配置場所を特定して、自動的に読み込んでくれます。

    オートローダーは ``src/`` ディレクトリの中の PHP クラスを見るようにも設定されています。\
    自動読み込みのために、クラス名とそのファイルのパスは次のような同じパターンになっています。

    .. code-block:: text

        Class Name:
            Acme\HelloBundle\Controller\HelloController
        Path:
            src/Acme/HelloBundle/Controller/HelloController.php

ソース (``src``) ディレクトリ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

簡単にいえば、\ ``src/`` ディレクトリは、アプリケーションを動かすために\ *あなたが書いた*\ 実際のコードすべてを含んでいます。\
例えば、\ PHP コード、テンプレート、設定ファイル、スタイルシートなどを含んでいます。\
開発するとき、ほとんどの作業は、このディレクトリに作った 1 つ以上のバンドルの中で完結しています。

では、\ :term:`バンドル`\ とは何でしょうか？

.. _page-creation-bundles:

バンドルシステム
-----------------

バンドルは他のソフトウェアでいうプラグインに似ていますが、それよりもずっと素晴らしいものです。\
重要な違いは Symfony2 では\ *すべて*\ がバンドルであることです。\
これにはコアフレームワークの機能もアプリケーションのために書いたコードも含みます。\
バンドルは Symfony2 において第一級市民なのです。\
これによって、\ `サードパーティのバンドル`_\ に構築された機能を使ったり、バンドルを配布したりすることが柔軟にできます。\
バンドルによってアプリケーションの中で有効にする機能を選択したり思うがままに最適化することが簡単にできます。

.. note::

   ここでは基本的なことのみ解説しています。\
   バンドルの構造やベストプラクティスについては、クックブックの\ :doc:`バンドル </cookbook/bundles/best_practices>`\ を参照してください。

バンドルは何か 1 つの機能を実装した単なるファイルの集合で、ディレクトリ構造です。\
``BlogBundle`` (ブログ) や ``ForumBundle`` (フォーラム) のような単位でバンドルを作成するでしょう (こういった機能を実装した OSS のバンドルも公開されています)。\
それぞれのディレクトリには、その機能に関連するPHP ファイルやテンプレート、スタイルシート、\ JavaScript\ 、テストやほかのすべてを含みます。\
ある機能のすべての面はバンドルに含まれており、すべての機能はバンドルの中に存在しています。

あるアプリケーションは、\ ``AppKernel`` クラスの ``registerBundles()`` メソッドの中で定義されたバンドルで構成されます。

.. code-block:: php

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            new Symfony\Bundle\FrameworkBundle\FrameworkBundle(),
            new Symfony\Bundle\SecurityBundle\SecurityBundle(),
            new Symfony\Bundle\TwigBundle\TwigBundle(),
            new Symfony\Bundle\MonologBundle\MonologBundle(),
            new Symfony\Bundle\SwiftmailerBundle\SwiftmailerBundle(),
            new Symfony\Bundle\DoctrineBundle\DoctrineBundle(),
            new Symfony\Bundle\AsseticBundle\AsseticBundle(),
            new Sensio\Bundle\FrameworkExtraBundle\SensioFrameworkExtraBundle(),
        );

        if (in_array($this->getEnvironment(), array('dev', 'test'))) {
            $bundles[] = new Acme\DemoBundle\AcmeDemoBundle();
            $bundles[] = new Symfony\Bundle\WebProfilerBundle\WebProfilerBundle();
            $bundles[] = new Sensio\Bundle\DistributionBundle\SensioDistributionBundle();
            $bundles[] = new Sensio\Bundle\GeneratorBundle\SensioGeneratorBundle();
        }

        return $bundles;
    }

``registerBundles()`` メソッドを用いることで、アプリケーションによって使われるバンドルを総合的にコントロールしています。

.. tip::

   バンドルは、オートロードできるよう設定されていれば\ *どこにでも*\ 置くことができます。

バンドルの作成
~~~~~~~~~~~~~~

Symfony Standard Edition には、バンドルのスケルトンを作成するためのコマンドがあります。\
バンドルを手動で作ることはとても簡単です。

バンドルシステムがどれほどシンプルかをお見せするために、\ ``AcmeTestBundle`` という名前で新しいバンドルを作り、有効化してみます。

.. tip::

    ``Acme`` の部分は単なるダミーの名前ですので、読者や読者の組織を表すベンダー名に置き換えてください(例えば ``ABCTestBundle`` は ``ABC`` という名前の会社のバンドルです)。

``src/Acme/TestBundle/`` ディレクトリを作成して、次のような ``AcmeTestBundle.php`` という名前の新しいファイルを追加してください。

.. code-block:: php

    // src/Acme/TestBundle/AcmeTestBundle.php
    namespace Acme\TestBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmeTestBundle extends Bundle
    {
    }

.. tip::

   ``AcmeTestBundle`` という名前は、標準的な\ :ref:`バンドル命名規則 <bundles-naming-conventions>`\ に従っています。\
   クラス名とファイル名を省略して、単純に ``TestBundle`` という名前のバンドルにすることもできます。

この空のクラスは新しいバンドルを作るために必要なただ 1 つの要素です。\
通常は空ですが、中身を記述してバンドルの動作のカスタマイズすることもできます。

バンドルを作成したので、\ ``AppKernel`` クラスで有効化しましょう。

.. code-block:: php

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            ...,
            // 自分のバンドルを追加する
            new Acme\TestBundle\AcmeTestBundle(),
        );
        // ...

        return $bundles;
    }

バンドル自体は何もしませんが、\ ``AcmeTestBundle`` は使う準備ができました。

これと同じくらい簡単にできるのですが、Symfony は基本的なバンドルのスケルトンを生成するためのコマンドラインインターフェースも提供しています。

.. code-block:: bash

    $ php app/console generate:bundle --namespace=Acme/TestBundle

このバンドルのスケルトンは、基本的なコントローラーやテンプレート、ルーティングのリソースをカスタマイズされた状態で生成します。\
Symfony2 のコマンドラインツールについては、後ほど詳しく学びます。

.. tip::

   新しいバンドルを作成したりサードパーティのバンドルを使うときは、いつも ``registerBundles()`` で有効にしなければなりません。\
   ``generate:bundle`` コマンドを使う場合は、有効化してくれます。

バンドルのディレクトリ構造
~~~~~~~~~~~~~~~~~~~~~~~~~~

バンドルのディレクトリ構造は簡単で柔軟性があります。\
標準では、バンドルシステムは、すべての Symfony2 バンドルの間でコードの一貫性を保ちやすいような規約に従っています。\
``AcmeHelloBundle`` を見てみてください。バンドルの最も一般的な要素で構成されています。

* ``Controller/`` はバンドルのコントローラーを含んでいます(例えば ``HelloController.php``)。

* ``DependencyInjection/`` には依存性注入向けのエクステンションクラスを格納します。\
  エクステンションはサービスコンフィギュレーションのインポートや、コンパイラーパスの登録などを行います。このディレクトリは必須ではありません。

* ``Resources/config/`` はルーティング設定を含む様々ば設定を格納しています(例えば ``routing.yml``)。

* ``Resources/views/`` はコントローラー名で整理されたテンプレートを保持しています(例えば ``Hello/index.html.twig``)。

* ``Resources/public/`` Web アセット(画像やスタイルシートなど)を含んでいます。\
  これらは ``assets:install`` コンソールコマンドによって、プロジェクトの ``web/`` ディレクトリの中にコピーあるいはシンボリックリンクされます。

* ``Tests/`` はバンドルのためのすべてのテストを含みます。

バンドルは実装する機能によって小さくなったり大きくなったりします。\
バンドルは必要とするファイルだけを含んでいるので、それ以外は含みません。

後の章で、データベースにオブジェクトを永続化する方法やフォームを作り検証する方法、アプリケーションで翻訳データを作る方法やテストの書き方など、より多くを学ぶでしょう。\
バンドルには、それぞれの役割を持ったディレクトリを作りそこにクラスを配置していきます。

アプリケーション設定
--------------------

あるアプリケーションは、そのアプリケーションのすべての機能を表すバンドルの集合で構成されます。\
それぞれのバンドルは YAML や XML\ 、\ PHP などで書かれた設定ファイルによってカスタマイズできます。\
標準では、メインの設定ファイルは ``app/config/`` ディレクトリにあり、選択した形式によって ``config.yml``\ 、\ ``config.xml``\ 、\ ``config.php`` というファイルになります。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: parameters.yml }
            - { resource: security.yml }

        framework:
            secret:          "%secret%"
            router:          { resource: "%kernel.root_dir%/config/routing.yml" }
            # ...

        # Twig Configuration
        twig:
            debug:            "%kernel.debug%"
            strict_variables: "%kernel.debug%"

        # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xmlns:twig="http://symfony.com/schema/dic/twig"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd
                                http://symfony.com/schema/dic/twig http://symfony.com/schema/dic/twig/twig-1.0.xsd">

            <imports>
                <import resource="parameters.yml" />
                <import resource="security.yml" />
            </imports>

            <framework:config secret="%secret%">
                <framework:router resource="%kernel.root_dir%/config/routing.xml" />
                <!-- ... -->
            </framework:config>

            <!-- Twig Configuration -->
            <twig:config debug="%kernel.debug%" strict-variables="%kernel.debug%" />

            <!-- ... -->
        </container>

    .. code-block:: php

        $this->import('parameters.yml');
        $this->import('security.yml');

        $container->loadFromExtension('framework', array(
            'secret'          => '%secret%',
            'router'          => array(
                'resource' => '%kernel.root_dir%/config/routing.php',
            ),
            // ...
            ),
        ));

        // Twig Configuration
        $container->loadFromExtension('twig', array(
            'debug'            => '%kernel.debug%',
            'strict_variables' => '%kernel.debug%',
        ));

        // ...

.. note::

   それぞれのファイル・形式をどうやって読み込むのかは次の `環境`_ の節で解説します。

``framework`` や ``twig`` のようなトップレベルのエントリは、それぞれ特定のバンドルのための設定に対応します。\
例えば、\ ``framework`` キーは Symfony の FrameworkBundle のための設定を定義していて、ルーティング、テンプレートやその他のコアシステムの設定を含んでいます。

さしあたっては、それそれの節において、特定の設定オプションについて心配する必要はありません。\
Standard Edition の設定ファイルはデフォルトで実用的な設定になっています。\
Symfony2 の各部分を読んだり探検したりするにつれて、それらの機能の設定オプションについて学べるでしょう。

.. sidebar:: 設定書式

    すべての章を通じて、すべての設定サンプルは 3 つの書式すべて(YAML\ 、\ XML\ 、\ PHP)で示します。\
    それぞれの書式に利点と欠点がありあます。選択肢はいくつかあります。

    * *YAML*: 完結で、きれいで、読みやすいです。詳細は ":doc:`/components/yaml/yaml_format`" を参照してください。

    * *XML*: 時には YAML よりも強力で、\ IDEの自動補完をサポートしています。

    * *PHP*: 非常に強力ですが、標準の設定形式よりは読みやすさが欠けます。

コンフィギュレーションのデフォルト値をダンプする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

コンソールの ``config:dump-reference`` コマンドを使うと、バンドルのデフォルトコンフィギュレーションを YAML 形式でダンプできます。\
FrameworkBundle のデフォルトコンフィギュレーションをダンプする例を次に示します。

.. code-block:: bash

    $ app/console config:dump-reference FrameworkBundle

バンドル名の代わりにエクステンションのエイリアスを指定してダンプすることもできます。

.. code-block:: bash

    $ app/console config:dump-reference framework

.. note::

    自分のバンドルにコンフィギュレーションを追加する方法については、\
    :doc:`バンドルのセマンティックコンフィギュレーション </cookbook/bundles/extension>`\
    を参照してください。

.. index::
   single: Environments; Introduction

.. _environments-summary:

環境
----

アプリケーションは様々な環境で実行することができます。\
環境が異なっていも同じ PHP コードを共有していますが(フロントコントローラーは別ですが)、別の設定を使います。
例えば、\ ``dev`` 環境は警告やエラーをログにかき込みますが、一方で ``prod`` 環境はエラーだけをログに書き込みます。\
``dev`` 環境では(開発者の利便性を考慮して)リクエストごとに同じファイルを再構築しますが、\ ``prod`` 環境ではキャッシュされます。すべての環境は同じサーバーに共存して同じアプリケーションを実行します。

Symfony2 のプロジェクトは一般的には３つの環境(``dev``\ 、\ ``test``\ 、\ ``prod``)で始まりますが、新しい環境を作ることも簡単です。
アプリケーションを違う環境で見る方法は簡単で、ブラウザでフロントコントローラーを変更することでできます。\
``dev`` 環境のアプリケーションを見るためには、開発用のフロントコントローラーでアプリケーションにアクセスします。

.. code-block:: text

    http://localhost/app_dev.php/hello/Ryan

プロダクト環境でどのように動くかを見たければ、代わりに ``prod`` のフロントコントローラーを呼び出してください。

.. code-block:: text

    http://localhost/app.php/hello/Ryan

``prod`` 環境では速度が最適化されており、コンフィギュレーション、ルーティング、Twig テンプレートはフラットな PHP ファイルへコンパイルされ、キャッシュされます。\
これらのファイルを変更した場合、\ ``prod`` 環境では変更を反映するために次のコマンドでキャッシュをクリアして再構築する必要があります。

.. code-block:: bash

    $ php app/console cache:clear --env=prod --no-debug

.. note::

   ``web/app.php`` ファイルを開いたら、明示的に ``prod`` 環境を使う設定がされているのがわかるでしょう。

   .. code-block:: php

       $kernel = new AppKernel('prod', false);

   このファイルをコピーして ``prod`` を別の値に変更すれば、新しい環境のための新しいフロントコントローラーが作成できます。

.. note::

    自動テストが走るときやブラウザから直接アクセス出来ないときは、\ ``test`` 環境が使われます。\
    詳しくは\ :doc:`テストの章 </book/testing>`\ を参照してください。

.. index::
   single: Environments; Configuration

環境設定
~~~~~~~~

``AppKernel`` クラスは、選択した設定ファイルを実際に読み込むことに責任があります。

.. code-block:: php

    // app/AppKernel.php
    public function registerContainerConfiguration(LoaderInterface $loader)
    {
        $loader->load(
            __DIR__.'/config/config_'.$this->getEnvironment().'.yml'
        );
    }

すでにご存知のとおり、\ ``.yml`` 拡張子は、設定を XML か PHP を使って書いていれば、\ ``.xml`` や ``.php`` に変更することができます。\
それぞれの環境は自分自身の設定ファイルを読み込むことにも注意してください。\
``dev`` 環境の設定ファイルについて考えてみましょう。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        imports:
            - { resource: config.yml }

        framework:
            router:   { resource: "%kernel.root_dir%/config/routing_dev.yml" }
            profiler: { only_exceptions: false }

        # ...

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <imports>
                <import resource="config.xml" />
            </imports>

            <framework:config>
                <framework:router
                    resource="%kernel.root_dir%/config/routing_dev.xml"
                />
                <framework:profiler only-exceptions="false" />
            </framework:config>

            <!-- ... -->

    .. code-block:: php

        // app/config/config_dev.php
        $loader->import('config.php');

        $container->loadFromExtension('framework', array(
            'router'   => array(
                'resource' => '%kernel.root_dir%/config/routing_dev.php',
            ),
            'profiler' => array('only-exceptions' => false),
        ));

        // ...

``imports`` キーは PHP の ``include`` 文と似たようなもので、メインの設定ファイル(``config.yml``)が最初に読み込まれることを保証しています。\
ファイルの残りの部分は、ログを増やしたり開発環境の助けとなる他の設定のために標準設定を微修正しています。

``prod`` と ``test`` 環境はいずれも次のようなルールに従っています:
それぞれの環境でベースとなる設定をインポートし、その後それぞれの環境に合わせて設定値を上書きします。\
これはある意味規約ではあるものの、ほとんどの設定を使いまわせて、環境間のちょっとした違いをカスタマイズするのに便利です。

まとめ
------

Symfony2 の様々な基本的な側面を見てきましたが、それらがいかに簡単で柔軟にできることが分かっていただけたでしょう。\
ここまでに\ *たくさんの*\ 機能がありましたが、次の基本的なポイントについて心にとどめておいてください:

* ページの作成は 3 つの手順からなり、\ **ルート**\ 、\ **コントローラー** \ 、そして(オプションですが)\ **テンプレート** を含みます。

* それぞれのプロジェクトはほんのいくつかのメインディレクトリで構成されます:
  ``web/`` ディレクトリ (Web アセットとフロントコントローラー)、\ ``app/`` ディレクトリ (設定)、\ ``src/`` ディレクトリ (読者のバンドル)、そして ``vendor/`` ディレクトリ (サードパーティのコード) です。

* Symfony2 フレームワークのコアを含む、\ Symfony2 の各々の機能は\ *バンドル*\ で整理されており、その機能のための構造化されたファイルの集合となっています。

* それぞれのバンドルの\ **設定**\ は、バンドルの\ ``Resources/config`` ディレクトリにあり、YAML か XML か PHP で設定できます。

* **アプリケーションコンフィギュレーション**\ は ``app/config`` ディレクトリにあります。

* それぞれの\ **環境**\ には対応するフロントコントローラーによってアクセスできます (例えば ``app.php`` と ``app_dev.php``)。\
  環境ごとの設定ファイルが読み込まれます。

以降の各章ではより強力なツールと高度な概念を紹介していきます。\
Symfony2 について詳しく知れば知るほど、アーキテクチャの柔軟性と高速にアプリケーションを開発できるパワーが分かってくるでしょう。

.. _`Twig`: http://twig.sensiolabs.org
.. _`サードパーティのバンドル`: http://knpbundles.com
.. _`Symfony Standard Edition`: http://symfony.com/download
.. _`Apache's DirectoryIndex documentation`: http://httpd.apache.org/docs/2.0/mod/mod_dir.html
.. _`Nginx HttpCoreModule location documentation`: http://wiki.nginx.org/HttpCoreModule#location

.. 2014/01/04 hidenorigoto d7c79ec86118286028ab05a82dcd142ff3f02786
