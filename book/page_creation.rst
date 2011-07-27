.. 2011/07/24 uechoco 6b7cca4814e689473ae6033da196d8591aeaa634
.. index::
   single: Page creation

Symfony2 でのページ作成
==========================

Symfony2 で新しいページを作成するには以下の２つの簡単なステップから構成されます。

* *ルート(route)の作成*: ルートは作成するページへの URL\ (例えば ``about``\ )を定義し、
  Symfony2 が実行すべきコントローラ(PHP 関数)を特定します。
  やってきたリクエストの URL がルートパターンにマッチするときに使われます。

* *コントローラの作成*: コントローラはやってきたリクエストを取り込んで、
  ユーザーに返却する ``Response`` オブジェクトに変換するような PHP 関数です。

このシンプルな方法は、ウェブの仕組みと一致していてとても美しいと言えます。
ウェブ上のあらゆる相互通信は HTTP リクエストによって開始されます。
アプリケーションの動作は、リクエストを読み取って適切な HTTP レスポンスを返すだけです。

Symfony2 はこの哲学に従って、ユーザーと複雑さが膨れ上がるようなアプリケーションを整理できるように
ツールや規約を提供しています。

とても簡単に聞こえますか？試してみましょう！

.. index::
   single: Page creation; Example

「Hello Symfony!」ページ
-------------------------

クラシックな「Hello World!」アプリケーションに派生してスタートしてみましょう。
このアプリケーションが作り終わったら、
以下の URL にアクセスするとあいさつページ(例えば「Hello Symfony」)を見ることができるでしょう。

.. code-block:: text

    http://localhost/app_dev.php/hello/Symfony

実際は、\ ``Symfony`` を挨拶されたい他の名前に置き換えることができるでしょう。
ページを作成するために、２つの簡単な手順を踏んでください。

.. note::

    チュートリアルではすでに Symfony2 をダウンロードしてウェブサーバーが設定されていることを想定しています。
    また、上の URL は、\ ``localhost`` が新しい Symfony2 プロジェクトの ``web`` ディレクトリに割り当てられている想定です。
    この手順についての詳しい説明は、\ :doc:`Symfony2 のインストール</book/installation>` を参照してください。

ページを作り始める前に: バンドルの作成
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ページを作り始める前に、\ *bandle(バンドル)* を作る必要があります。
Symfony2 では、アプリケーション内のすべてのコードはバンドル内にあることを除けば
:term:`bundle` はプラグインのようなものと言えます。

バンドルは、固有の機能に関連するすべてのものを格納している単なるディレクトリに過ぎません。
これには PHP クラスや設定だけでなく、スタイルシートや Javascript のファイルなども含みます(\ :ref:`page-creation-bundles` を参照)。

``AcmeHelloBundle`` (この章で作る予定のお遊びバンドル) と呼ぶバンドルを作成するために、
次のコマンドを実行し、画面の指示に従ってください(標準のオプションを使います)。

.. code-block:: bash

    php app/console generate:bundle --namespace=Acme/HelloBundle --format=yml

裏側では ``src/Acme/HelloBundle`` にバンドルのディレクトリが作成されます。
カーネルにバンドルを登録するために ``app/AppKernel.php`` にも自動的に行が追加されます。

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Acme\HelloBundle\AcmeHelloBundle(),
        );
        // ...
        
        return $bundles;
    }

バンドルのセットアップされたので、
そのバンドルの中にアプルケーションを構築することができるようになりました。

ステップ 1: ルートの作成
~~~~~~~~~~~~~~~~~~~~~~~~

標準では、\ Symfony2 アプリケーションのルーティング設定は
``app/config/routing.yml`` にあります。
Symfony2 のすべての設定と同様にに、ルートの設定することにとらわれずに XML か PHP を選択することも出来ます。

メインのルーティングファイルを見ると、
``AcmeHelloBundle`` を作ったときに Symfony がすでにエントリを追加しているのがわかるでしょう。

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        AcmeHelloBundle:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"
            prefix:   /

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" prefix="/" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->addCollection(
            $loader->import('@AcmeHelloBundle/Resources/config/routing.php'),
            '/',
        );

        return $collection;

このエントリはかなり基本的ことです。ルーティングの設定を ``Resources/config/routing.yml`` から
読み込むことを Symfony に伝えています。このファイルは ``AcmeHelloBundle`` の中にあります。
これは、ルーティング設定を直接 ``app/config/routing.yml`` に置くか、
アプリケーションのどこにでもルートを整理することができ、ここからインポートすることを意味しています。

これでバンドルから ``routing.yml`` ファイルがインポートされました。
これから作ろうとしているページのURLを定義した新しいルートを追加しましょう。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/routing.yml
        hello:
            pattern:  /hello/{name}
            defaults: { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="hello" pattern="/hello/{name}">
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

このルーティングは２つの基本的な項目から構成されています。１つ目は ``pattern`` で、
このルートがマッチする URL のことです。２つ目は ``defaults`` 配列で、
実行されるべきコントローラを特定しています。
パターンの中のプレースホルダー文法(``{name}``)はワイルドカードです。
`/hello/Ryan`` や ``/hello/Fabien`` や他の同様の URL がマッチすることを意味しています。
``{name}`` プレースホルダーパラメータも、値をあいさつに使えるようにコントローラに通ります。

.. note::

  ルーティングシステムにはアプリケーションの URL 構造を柔軟かつパワフルにつくるための
  より多くのすばらしい機能があります。
  より詳しい情報は :doc:`ルーティング</book/routing>` についてのすべての章を参照してください。

ステップ2: コントローラの作成
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

アプリケーションが ``/hello/Ryan`` のような URL を扱うようになると、
``hello`` ルートがマッチしてフレームワークが ``AcmeHelloBundle:Hello:index`` コントローラを実行します。
ページ作成手順の２つ目のステップはそのコントローラを作成することです。

``AcmeHelloBundle:Hello:index`` はコントローラの\ *論理*\ 名で、
``Acme\HelloBundle\Controller\Hello`` クラスの``indexAction`` メソッドにマッピングされています。
``AcmeHelloBundle`` の中にこのファイルを作成することから始めましょう。

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
    }

実は、コントローラは、あなたが作成して Symfony が実行するメソッドに過ぎません。
コントローラは、リクエストされたリソースを構築し準備し、それらの情報を使うところです。
いくらかの高度な場合を除けば、コントローラの生成物は常に同じで、
Symfony2 の ``Response`` オブジェクトです。

``hello`` ルートがマッチしたときに Symfony が実行する ``indexAction`` メソッドを作りましょう。

    // src/Acme/HelloBundle/Controller/HelloController.php

    // ...
    class HelloController
    {
        public function indexAction($name)
        {
            return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

コントローラは単純で、 ``Response`` オブジェクトを作成します。
このオブジェクトの最初の引数は、レスポンスで使われるコンテンツです
(例として小さなHTMLページを想定しています)。

おめでとう！ルートとコントローラを１つずつ作っただけで、すでに実用的なページができあがりました！
正しくセットアップされていれば、アプリケーションがあいさつを返してくれるでしょう:

.. code-block:: text

    http://localhost/app_dev.php/hello/Ryan

オプションにはなりますが、一般的には３つ目のステップとしてテンプレートの作成があります。

.. note::

   ページを作成するときにはコントローラは、書いたコードのメインのエントリポイントになり、
  重要な構成要素でもあります。詳しくは :doc:`コントローラの章</book/controller>` を参照してください。

オプションのステップ3: テンプレートの作成
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

テンプレートは、\ HTML コードなどのプレゼンテーションを別のファイルに分けることが出来、
ページレイアウトの異なる部分で再利用出来るようになります。
コントローラの中に HTML を書く代わりにテンプレートを描画します。

.. code-block:: php
    :linenos:

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
            return $this->render('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

            // render a PHP template instead
            // return $this->render('AcmeHelloBundle:Hello:index.html.php', array('name' => $name));
        }
    }

.. note::

   ``render()`` メソッドを使うために、コントローラは
   ``Symfony\Bundle\FrameworkBundle\Controller\Controller`` クラス
   (API docs: :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`)を
   拡張する必要があります。このクラスは、コントローラの中でよく使われる動作の
   ショートカットを追加しています。上のサンプルでは実装済みで、
   ４行目に ``use`` 文を追加して、６行目でクラスを拡張しています。

``render()`` メソッドは、\ ``Response`` オブジェクトを作成しますが、
このオブジェクトは描画されたテンプレートの内容で満たされています。
他のコントローラと同様に、最終的には ``Response`` オブジェクトを返しています。

テンプレートの描画について、２つの異なる例があることに注意してください。
標準では Symfony2 は ２つの異なるテンプレート言語をサポートしています。
１つはクラシックな PHP テンプレートで、もう１つは簡潔ですが強力な `Twig`_ テンプレートです。
心配しないでください。同じプロジェクト内でどちらかあるいはどちらも自由に選べます。

このコントローラは ``AcmeHelloBundle:Hello:index.html.twig`` テンプレートを描画しますが、
次のような命名規則を使っています:

    **バンドル名**:**コントローラ名**:**テンプレート名**

これはテンプレートの *論理的な* 名前で、次のような規則を用いた物理パスとのマッピングです:

    **/path/to/BundleName**/Resources/views/**ControllerName**/**TemplateName**

今回の場合は ``AcmeHelloBundle`` がバンドル名、\ ``Hello`` がコントローラ名、
そして ``index.html.twig`` がテンプレート名です。

.. configuration-block::

    .. code-block:: jinja
       :linenos:

        {# src/Acme/HelloBundle/Resources/views/Hello/index.html.twig #}
        {% extends '::layout.html.twig' %}

        {% block body %}
            Hello {{ name }}!
        {% endblock %}

    .. code-block:: php

        <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
        <?php $view->extend('::layout.html.php') ?>

        Hello <?php echo $view->escape($name) ?>!

Twig テンプレートを１行１行見ていきましょう。

* *line 2*: ``extends`` トークンは親のテンプレートを定義します。
  親のテンプレートでは明示的にレイアウトファイルがどこに置かれるかを定義しています。

* *line 4*: ``block`` トークンは ``body`` という名前のブロックの中に挿入されるものを
  示しています。ご覧のとおり、親のテンプレート(``layout.html.twig``) は
  ``body`` という名前のブロックが最終的に描画されることに対して責任を負います。

親のテンプレートである ``::layout.html.twig`` は、
名前から **バンドル名** と **コントローラ名** が無くなっていて、
先頭が二重コロン(``::``)になっています。
これはテンプレートがバンドルの外に存在していて、\ ``app`` ディレクトリの中にあることを意味しています。

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/layout.html.twig #}
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title>{% block title %}Hello Application{% endblock %}</title>
            </head>
            <body>
                {% block body %}{% endblock %}
            </body>
        </html>

    .. code-block:: php

        <!-- app/Resources/views/layout.html.php -->
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title><?php $view['slots']->output('title', 'Hello Application') ?></title>
            </head>
            <body>
                <?php $view['slots']->output('_content') ?>
            </body>
        </html>

ベースのテンプレートファイルは HTML レイアウトを定義し、
``index.html.twig`` テンプレート内で定義した ``body`` ブロックを秒しています。
このテンプレートは ``title`` ブロックも描画していて、\ ``index.html.twig`` テンプレート内で
定義することもできます。\ ``title`` ブロックを子テンプレートでで定義しなければ
初期値で「Hello Application」となります。

テンプレートはページのコンテンツを描画し整理するための強力な方法です。
テンプレートは HTML マークアップから CSS コード、
あるいはコントローラが返したいあらゆるものを描画できます。

リクエストのライフサイクルにおいて、テンプレートエンジンは単なるオプションツールです。
各コントローラの最終目標を思い出すと ``Response`` オブジェクトを返却することです。
テンプレートは ``Response`` オブジェクトのコンテンツを作成するための強力で、しかしオプションの、ツールです。

.. index::
   single: Directory Structure

ディレクトリ構造
-----------------------

ほんのいくつかの節を経たことで、 Symfony2 においてページを作り描画する作業の裏側にある哲学をもう理解できました。
また Symfony2 のプロジェクトがどのように構造化され整理されているかも分かり始めてきたでしょう。
この節の終わりまでには様々なファイルがどこにあり、どこに置き、なぜそこに置くのかがわかるでしょう。

あらゆることに柔軟に対応できるのですが、標準では各 Symfony の :term:`アプリケーション` は
共通の基本的なディレクトリ構造を持っていて、この構造は推奨されています。

* ``app/``: アプリケーション設定を含むディレクトリ

* ``src/``: プロジェクトのすべての PHP コードは このディレクトリの下に格納されます

* ``vendor/``: 慣例ではあらゆるベンダーライブラリはここに置かれます

* ``web/``: ここはウェブルートディレクトリで、公開してアクセス可能なファイルはここに含めます

ウェブディレクトリ
~~~~~~~~~~~~~~~~~~~

ウェブルートディレクトリは公開する静的なファイルすべてを置く場所です。
画像やスタイルシート、そして JavaScript も含みます。
また次のような :term:`フロントコントローラ` を置く場所でもあります:

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

フロントコントローラは(``app.php`` を例にすると)\ Symfony2 を使うときに実行される
PHP ファイルで、アプリケーションを起動するために ``AppKernel`` クラスを使います。

.. tip::

    フロントコントローラを持っているということは、典型的なフラットな PHP アプリケーション内で使うのとは違い、
    より柔軟な URL に対応できることを意味しています。フロントコントローラを使うとき、
    URL 次のように書きます。

    .. code-block:: text

        http://localhost/app.php/hello/Ryan

    フロントコントローラの ``app.php`` が実行され、\ "内部的な:" URL の
    ``/hello/Ryan`` はルートの設定を使って内部的にルートされます。
    Apache の ``mod_rewrite`` ルールを使えば、次のような URL でファイル名を特定しなくても
    ``app.php`` を実行させることができます。

    .. code-block:: text

        http://localhost/hello/Ryan

フロントコントローラはすべてのリクエストの扱いにおいての重要なポイントではありますが、
フロントコントローラを修正したり、その存在自体をかんがえることさえもほとんどありません。
フロントコントローラについていは `環境`_ 節で再び簡単に触れようと思います。

アプリケーション (``app``) ディレクトリ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

フロントコントローラで見たように、\ ``AppKernel`` クラスはアプリケーションのメインの
エントリポイントで、すべての設定に責任を持ちます。\ ``app/`` ディレクトリの中に
格納されているような設定です。

このクラスは２つのメソッドを実装しなければならず、
これらは Symfony がアプリケーションについて知るために必要なすべての定義です。
開発を始めるときはこれらのメソッドに心配をする必要さえありません。
Symfony が実用的な標準設定をしてくれています。

* ``registerBundles()``: アプリケーションで実行する必要があるバンドルの配列を返します。
  (:ref:`page-creation-bundles` を参照);

* ``registerContainerConfiguration()``: メインアプリケーションのリソースファイルを読み込みます。
  (see the `アプリケーション設定`_ section).

日常的な開発においては、\ ``app/config/`` ディレクトリの中の設定やルーティングファイルを
編集するために ``app/`` ディレクトリをよく使うでしょう(`アプリケーション設定`_ を参照)。
また ``app/`` ディレクトリは、アプリケーションキャッシュディレクトリ(``app/cache``)や
ログディレクトリ(``app/logs``)、そしてテンプレート(``app/Resources``)などの
アプリケーションレベルのリソースファイルなども含みます。
これらのディレクトリについては後の章でより詳しく学べるでしょう。

.. _autoloading-introduction-sidebar:

.. sidebar:: 自動読み込み(オートローディング)

    Symfony がロードされるとき、\ ``app/autoload.php`` という特別なファイルが読み込まれます。
    このファイルは ``src/`` ディレクトリからアプリケーションのファイルを、\ ``vendor/`` ディレクトリから
    サードパーティのライブラリを自動読み込みします。

    オートローダーがあるので、\ ``include`` や ``require`` を書くことに心配になる必要は全くありません。
    その代わりに、\ Symfony2 がクラスの置かれている場所から決定される名前空間を使って、
    必要なクラスを自動的に読み込んでくれます。

    オートローダーは ``src/`` ディレクトリの中の PHP クラスを見るようにも設定されています。
    自動読み込みのために、クラス名とそのファイルのパスは次のような同じパターンになっています。

    .. code-block:: text

        Class Name:
            Acme\HelloBundle\Controller\HelloController
        Path:
            src/Acme/HelloBundle/Controller/HelloController.php

    一般的には、\ ``app/autoload.php`` ファイルについて気にする必要があるのは、
    ``vendor/`` ディレクトリのサードパーティのライブラリを新しく読み込む時だけです。
    自動読み込みの詳細は、\ :doc:`どうやってクラスを自動読み込みするか</cookbook/tools/autoloader>`
    を参照してください。

ソース (``src``) ディレクトリ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

簡単にいえば、\ ``src/`` ディレクトリは、
アプリケーションを動かすための *あなたが書いた* 実際のコードすべてを含んでいます。
例えば、\ PHP コード、テンプレート、設定ファイル、スタイルシートなどを含んでいます。
開発するとき、ほとんどの作業は、このディレクトリに作った１つ以上のバンドルの中で完結しています。

では、\ :term:`バンドル`\ とはなんでしょうか？

.. _page-creation-bundles:

バンドルシステム
-----------------

バンドルは他のソフトウェアでいうプラグインに似ていますが、それよりもずっと素晴らしいものです。
重要な違いは Symfony2 では *すべて* がバンドルであることです。
これにはコアフレームワークの機能もアプリケーションのために書いたコードも含みます。
バンドルは Symfony2 において第一級市民なのです。
これによって、\ `サードパーティのバンドル`_ に構築された機能を使ったり、
バンドルを配布したりすることが柔軟にできます。
バンドルによってアプリケーションの中で有効にする機能を選択したり思うがままに最適化することが簡単にできます。

.. note::

   ここでは基本的なことを学ぶことになると思いますが、
   クックブックのエントリはすべて :doc:`bundles</cookbook/bundles/best_practices>` の構造やベストプラクティスに向けられています。

バンドルは１つの機能を実装したディレクトリの中の構造化された単なるファイルの集合です。
``BlogBundle`` や ``ForumBundle``\  、あるいはオープンソースのバンドルなどの管理しているバンドルをつくるでしょう。
それぞれのディレクトリはその機能に関連するすべてのファイルを含んでいます。
PHP ファイルやテンプレート、スタイルシート、\ JavaScript\ 、テストやほかのすべてを含みます。
ある機能のすべての面はバンドルに含まれており、すべての機能はバンドルの中に存在しています。

あるアプリケーションは、\ ``AppKernel`` クラスの ``registerBundles()`` メソッドの中で定義されたバンドルで構成されます。

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
            new JMS\SecurityExtraBundle\JMSSecurityExtraBundle(),
        );

        if (in_array($this->getEnvironment(), array('dev', 'test'))) {
            $bundles[] = new Acme\DemoBundle\AcmeDemoBundle();
            $bundles[] = new Symfony\Bundle\WebProfilerBundle\WebProfilerBundle();
            $bundles[] = new Sensio\Bundle\DistributionBundle\SensioDistributionBundle();
            $bundles[] = new Sensio\Bundle\GeneratorBundle\SensioGeneratorBundle();
        }

        return $bundles;
    }

``registerBundles()`` メソッドを用いることで、アプリケーションによって使われるバンドルを
総合的にコントロールしています。

.. tip::

   バンドルは、(``app/autoload.php`` によってオートローダを設定して)自動読み込みが出来さえすれば
   *どこにでも* 置くことができます。

バンドルの作成
~~~~~~~~~~~~~~~~~

Symfony スタンダードエディションには、ちゃんと動作するバンドルとつくるためのタスクが付属しています。
もちろんバンドルを手動で作ることもとても簡単です。

バンドルシステムがどれほどシンプルかをお見せするために、
``AcmeTestBundle`` という名前で新しいバンドルを作り、有効化してみます。

.. tip::

    ``Acme`` の部分は単なるダミーの名前ですので、読者や読者の組織を表すベンダー名に
    置き換えてください(例えば ``ABCTestBundle`` は ``ABC`` という名前の会社のバンドルです)。

``src/Acme/TestBundle/`` ディレクトリを作成して、次のような ``AcmeTestBundle.php`` という名前の
新しいファイルを追加してください。

    // src/Acme/TestBundle/AcmeTestBundle.php
    namespace Acme\TestBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmeTestBundle extends Bundle
    {
    }

.. tip::

   ``AcmeTestBundle`` という名前は、標準的な :ref:`バンドル命名規則<bundles-naming-conventions>` に従っています。
   クラス名とファイル名を省略して、単純に ``TestBundle`` という名前のバンドルにすることもできます。

この空のクラスは新しいバンドルを作るために必要なただ１つの要素です。
通常はからですが、このクラスはバンドルの動作をカスタマイズできてとても強力です。

バンドルを作成したので、\ ``AppKernel`` クラスで有効化しまししょう。

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...

            // register your bundles
            new Acme\TestBundle\AcmeTestBundle(),
        );
        // ...

        return $bundles;
    }

バンドル自体は何もしませんが、\ ``AcmeTestBundle`` は使う準備ができました。

これと同じくらい簡単にできるのですが、
\ Symfony は基本的なバンドルのスケルトンを生成するための
コマンドラインインターフェースも提供しています。

.. code-block:: bash

    php app/console generate:bundle --namespace=Acme/TestBundle

このバンドルのスケルトンは、基本的なコントローラやテンプレート、
ルーティングのリソースをカスタマイズされた状態で生成します。
Symfony2 のコマンドラインツールについては、後ほど詳しく学びます。

.. tip::

   新しいバンドルを作成したりサードパーティのバンドルを使うときは、
   いつも ``registerBundles()`` で有効にしなければなりません。
   ``generate:bundle`` コマンドを使う場合は、有効化してくれます。

バンドルのディレクトリ構造
~~~~~~~~~~~~~~~~~~~~~~~~~~

バンドルのディレクトリ構造は簡単で柔軟性があります。
標準では、バンドルシステムは、すべての Symfony2 バンドルの間で
コードの一貫性を保ちやすいような規約に従っています。
``AcmeHelloBundle`` を見てみてください。バンドルの最も一般的な要素で構成されています。

* ``Controller/`` はバンドルのコントローラを含んでいます(例えば ``HelloController.php``)。

* ``Resources/config/`` はルーティング設定を含む様々ば設定を格納しています(例えば ``routing.yml``)。

* ``Resources/views/`` はコントローラ名で整理されたテンプレートを保持しています(例えば ``Hello/index.html.twig``)。

* ``Resources/public/`` ウェブアセット(画像やスタイルシートなど)を含んでいます。
  これらは ``assets:install`` コンソールコマンドによって、プロジェクトの ``web/`` ディレクトリの中に
  コピーあるいはシンボリックリンクされます。

* ``Tests/`` はバンドルのためのすべてのテストを含みます。

バンドルは実装する機能によって小さくなったり大きくなったりします。
バンドルは必要とするファイルだけを含んでいるので、それ以外は含みません。

この本を進んでいくにつれて、データベースにオブジェクトを永続化する方法やフォームを作り検証する方法、
アプリケーションで翻訳データを作る方法やテストの書き方など、より多くを学ぶでしょう。
これらはそれぞれバンドルのなかで各々の配置があり、役割をもっています。

アプリケーション設定
-------------------------

あるアプリケーションは、そのアプリケーションのすべての機能を表すバンドルの集合で構成されます。
それぞれのバンドルは YAML や XML\ 、\ PHP などで書かれた設定ファイルによってカスタマイズできます。
標準では、メインの設定ファイルは ``app/config/`` ディレクトリにあり、
それぞれ ``config.yml``\ 、\ ``config.xml``\ 、\ ``config.php`` と呼ばれ、
選んだ形式によって書式が決まっています。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: parameters.ini }
            - { resource: security.yml }
        
        framework:
            secret:          %secret%
            charset:         UTF-8
            router:          { resource: "%kernel.root_dir%/config/routing.yml" }
            form:            true
            csrf_protection: true
            validation:      { enable_annotations: true }
            templating:      { engines: ['twig'] } #assets_version: SomeVersionScheme
            session:
                default_locale: %locale%
                auto_start:     true

        # Twig Configuration
        twig:
            debug:            %kernel.debug%
            strict_variables: %kernel.debug%

        # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <imports>
            <import resource="parameters.ini" />
            <import resource="security.yml" />
        </imports>
        
        <framework:config charset="UTF-8" secret="%secret%">
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
            <framework:form />
            <framework:csrf-protection />
            <framework:validation annotations="true" />
            <framework:templating assets-version="SomeVersionScheme">
                <framework:engine id="twig" />
            </framework:templating>
            <framework:session default-locale="%locale%" auto-start="true" />
        </framework:config>

        <!-- Twig Configuration -->
        <twig:config debug="%kernel.debug%" strict-variables="%kernel.debug%" />

        <!-- ... -->

    .. code-block:: php

        $this->import('parameters.ini');
        $this->import('security.yml');

        $container->loadFromExtension('framework', array(
            'secret'          => '%secret%',
            'charset'         => 'UTF-8',
            'router'          => array('resource' => '%kernel.root_dir%/config/routing.php'),
            'form'            => array(),
            'csrf-protection' => array(),
            'validation'      => array('annotations' => true),
            'templating'      => array(
                'engines' => array('twig'),
                #'assets_version' => "SomeVersionScheme",
            ),
            'session' => array(
                'default_locale' => "%locale%",
                'auto_start'     => true,
            ),
        ));

        // Twig Configuration
        $container->loadFromExtension('twig', array(
            'debug'            => '%kernel.debug%',
            'strict_variables' => '%kernel.debug%',
        ));

        // ...

.. note::

   それぞれのファイル・形式をどうやって読み込むのかは次の `環境`_ の節で学べるでしょう。

``framework`` や ``twig`` のようなトップレベルのエントリは、
それぞれ特定のバンドルのための設定を定義しています。
例えば、\ ``framework`` キーは Symfony の ``FrameworkBundle`` のための設定を定義していて、
ルーティング、テンプレート、そしてほかのコアシステムの設定を含んでいます。

さしあたっては、それそれの節において、特定の設定オプションについて心配する必要はありません。
設定ファイルは実用的な標準設定で同梱されています。
Symfony2 の各部分を読んだり探検したりするにつれて、
それらの機能の設定オプションについて学べるでしょう。

.. sidebar:: 設定書式

    すべての章を通じて、すべての設定サンプルは３つの書式すべて(YAML\ 、\ XML\ 、\ PHP)で示します。
    それぞれの書式に利点と欠点がありあます。選択肢はいくつかあります。

    * *YAML*: 完結で、きれいで、読みやすいです。

    * *XML*: 時には YAML よりも強力で、\ IDEの自動補完をサポートしています。

    * *PHP*: 非常の強力ですが、標準の設定形式よりは読みやすさが欠けます。

.. index::
   single: Environments; Introduction

.. _environments-summary:

環境
------------

アプリケーションは様々な環境で実行することができます。
環境が異なっていも同じ PHP コードを共有していますが(フロントコントローラは別ですが)、
別の設定を使います。例えば、\ ``dev`` 環境は警告やエラーをログにかき込みますが、
一方で ``prod`` 環境はエラーだけをログに書き込みます。
``dev`` 環境では(開発者の利便性を考慮して)リクエストごとに同じファイルを再構築しますが、
``prod`` 環境ではキャッシュされます。すべての環境は同じサーバーに共存して同じアプリケーションを実行します。

Symfony2 のプロジェクトは一般的には３つの環境(``dev``\ 、\ ``test``\ 、\ ``prod``)で始まりますが、
新しい環境を作ることも簡単です。アプリケーションを違う環境で見る方法は簡単で、
ブラウザでフロントコントローラを変更することでできます。
``dev`` 環境のアプリケーションを見るためには、
開発用のフロントコントローラでアプリケーションにアクセスします。

.. code-block:: text

    http://localhost/app_dev.php/hello/Ryan

プロダクト環境でどのように動くかを見たければ、
代わりに ``prod`` のフロントコントローラを呼び出してください。

.. code-block:: text

    http://localhost/app.php/hello/Ryan

.. note::

   ``web/app.php`` ファイルを開いたら、明示的に ``prod`` 環境を使う設定がされているのがわかるでしょう。

       $kernel = new AppKernel('prod', false);

   このファイルをコピーして ``prod`` を別の値に変更すれば、
   新しい環境のための新しいフロントコントローラが作成できます。

``prod`` 環境は速度を最適化されているので、設定やルーティング、\ Twig テンプレートは
フラットな PHP クラスにコンパイルされ、キャッシュされます。
``prod`` 環境の表示結果を変更したいときは、
これらのキャッシュファイルをクリアする必要がありますが、
次のコマンドでこれらを再構築できます。

    php app/console cache:clear --env=prod

.. note::

    自動テストが走るときやブラウザから直接アクセス出来ないときは、\ ``test`` 環境が使われます。
    詳しくは :doc:`テストの章</book/testing>` を参照してください。

.. index::
   single: Environments; Configuration

環境設定
~~~~~~~~~~~~~~

``AppKernel`` クラスは、選択した設定ファイルを実際に読み込むことに責任があります。

    // app/AppKernel.php
    public function registerContainerConfiguration(LoaderInterface $loader)
    {
        $loader->load(__DIR__.'/config/config_'.$this->getEnvironment().'.yml');
    }

すでにご存知のとおり、\ ``.yml`` の拡張子は、
設定を XML か PHP を使って書いていれば、
``.xml`` や ``.php`` に変更することができます。
それぞれの環境は自分自身の設定ファイルを読み込むことにも注意してください。
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
        <imports>
            <import resource="config.xml" />
        </imports>

        <framework:config>
            <framework:router resource="%kernel.root_dir%/config/routing_dev.xml" />
            <framework:profiler only-exceptions="false" />
        </framework:config>

        <!-- ... -->

    .. code-block:: php

        // app/config/config_dev.php
        $loader->import('config.php');

        $container->loadFromExtension('framework', array(
            'router'   => array('resource' => '%kernel.root_dir%/config/routing_dev.php'),
            'profiler' => array('only-exceptions' => false),
        ));

        // ...

``imports`` キーは PHP の ``include`` 文と似たようなもので、
メインの設定ファイル(``config.yml``)が最初に読み込まれることを保証しています。
ファイルの残りの部分は、ログを増やしたり開発環境の助けとなる他の設定のために
標準設定を微修正しています。

``prod`` と ``test`` 環境は両方共次のような同じモデルに従っています:
それぞれの環境はベース設定をインポートし、それぞれの環境に合わせて設定値を変更します。
これはある意味規約ではあるものの、ほとんどの設定を使いまわせて、
環境間のちょっとした違いをカスタマイズすることができます。

要約
-------

おめでとう！ Symfony2 の様々な基本的な側面を見てきましたが、
それらがいかに簡単で柔軟にできることが分かっていただけたでしょう。
ここまでに *たくさんの* 機能がありましたが、
次の基本的なポイントについて心にとどめておいてください:

* ページの作成は３つの手順からなり、\ **ルート**\ 、\ **コントローラ** \ 、
  そして(オプションですが)\ **テンプレート** を含みます。

* それぞれのプロジェクトはほんのいくつかのメインディレクトリで構成されます:
  ``web/`` ディレクトリ(ウェブアセットとフロントコントローラ)、
  ``app/`` ディレクトリ(設定)、\ ``src/`` ディレクトリ(読者のバンドル)、
  そして ``vendor/`` ディレクトリ(サードパーティのコード)です。
  ベンダーライブラリをアップデートするために使う ``bin/`` ディレクトリも
  含まれます。

* Symfony2 フレームワークのコアを含む、\ Symfony2 の各々の機能は *バンドル* で整理されており、
  その機能のための構造化されたファイルの集合となっています。

* それぞれのバンドルの\ **設定**\ は、\ ``app/config`` ディレクトリにあり、
  YAML か XML か PHP で設定できます。

* それぞれの\ **環境**\ は別のフロントコントローラによってアクセスできます
  (例えば ``app.php`` と ``app_dev.php``)。そして異なる設定ファイルを読み込みます。

ここからは、各章ではより強力なツールと高度な概念を紹介していきます。
Symfony2 について詳しく知れば知るほど、アーキテクチャの柔軟性と
高速アプリケーションを開発できるパワーが分かってくるでしょう。

.. _`Twig`: http://www.twig-project.org
.. _`サードパーティのバンドル`: http://symfony2bundles.org/
.. _`Symfony スタンダードエディション`: http://symfony.com/download
