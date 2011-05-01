.. 2011/05/11 hidenorigoto 778e9aca

Symfony2 の全体像
=================

今から 10 分で、Symfony2 を使い始める準備をしましょう！
この章では、Symfony2 の基盤となっているとても重要な概念について解説します。
Symfony2 の単純なプロジェクトの構造を見ていきながら、これらの概念について説明します。

すでに Web アプリケーションフレームワークを使ったことのある方なら、Symfony2 にすぐに慣れることができるでしょう。
はじめての Web アプリケーションフレームワークについて学ぶ方は、新しい Web アプリケーションの開発方法をしっかりと学習してください！

.. tip:

    フレームワークを「なぜ」「いつ」使うのかを学びたい方は、\ "`Symfony in 5 minutes`_" ドキュメントをお読みください。

Symfony2 をダウンロードする
---------------------------

最初に、Web サーバー（たとえば Apache）のインストールと設定が完了しており、PHP 5.3.2 以降のバージョンがインストールされていることを確認してください。

次に、\ "`Symfony2 Standard Edition`_" をダウンロードしましょう。
これは Symfony の\ :term:`ディストリビューション`\ で、もっともよく利用されるユースケース向けにあらかじめ設定されています。
また、Symfony2 の使い方を学ぶためのデモンストレーションコードも含まれています。
ダウンロード画面で *vendors* が含まれているアーカイブを選択すると、より手軽に環境の準備が整います。

ダウンロードしたアーカイブを Web サーバーのドキュメントルートディレクトリで展開すると、次のような構成の \ ``Symfony/``\ ディレクトリができます。

.. code-block:: text

    www/ <- Web サーバーのドキュメントルートディレクトリ
        Symfony/ <- ダウンロードしたアーカイブを展開したもの
            app/
                cache/
                config/
                logs/
            src/
                Acme/
                    DemoBundle/
                        Controller/
                        Resources/
            vendor/
                symfony/
                doctrine/
                ...
            web/
                app.php

設定を確認する
--------------

Symfony2 には、ビジュアルにサーバーの設定を確認するテスターが組み込まれており、これを使うことで Web サーバーや PHP の設定の間違いによる混乱を回避できます。
次の URL にアクセスすると、使っている環境の診断結果が表示されます。

.. code-block:: text

    http://localhost/Symfony/web/config.php

問題がある設定のリストが表示されるので、それぞれ解決してください。
推奨される設定と異なっている箇所も表示されるので、可能であればそれらも変更してください。
すべての準備が整ったら、\ "Go to the Welcome page" というリンクをクリックして、最初の Symfony2 Web ページへアクセスしてみましょう。

.. code-block:: text

    http://localhost/Symfony/web/app_dev.php/

ここまでの準備が完了していれば、次のような Welcome 画面が表示されるはずです。

.. image:: /images/quick_tour/welcome.jpg

Symfony2 の基盤技術を理解する
-----------------------------

フレームワークを利用することの主要な目的の 1 つに、\ `関心事の分離`_\ という概念があります。
この概念によりコードは組織化され、たとえばデータベース呼び出し、HTML タグ、ビジネスロジックが同一のスクリプトに混在するような状況を回避して、長期間に渡るアプリケーションの進化にも対応しやすくなります。
Symfony2 を使ったアプリケーションの開発でこの目的を達成するためには、まず新しい基盤技術の概念や用語を学んでおく必要があります。

.. tip::

    同一のスクリプトにすべてを混在して記述するよりもフレームワークを使うことの方が優れている理由を知りたい方は、\ ":doc:`/book/from_flat_php_to_symfony2`" の章をお読みください。

Symfony2 のディストリビューションには、Symfony2 の主要なコンセプトを学習するのに使えるサンプルコードが含まれています。
次の URL にアクセスすると、Symfony2 による挨拶メッセージが表示されます（\ *Fabien* の部分は自分の名前に置き換えてください）

.. code-block:: text

    http://localhost/Symfony/web/app_dev.php/demo/hello/Fabien

これはどういった仕組みなんでしょうか？ まずは URL のパーツを順に見ていきましょう。

* ``app_dev.php``: これは\ :term:`フロントコントローラ`\ です。
  アプリケーションごとの独立したエントリポイントで、ユーザーからのすべてのリクエストに応答します。

* ``/demo/hello/Fabien``: この部分は、ユーザーがアクセスしようとしているリソースの\ *バーチャルパス*\ です。

あなたの開発者としての責務は、ユーザーの\ *リクエスト* (``/demo/hello/Fabien``) を (``Hello Fabien!``) に関連付けられた\ *リソース*\ にマップするコードを記述することです。

ルーティング
~~~~~~~~~~~~

Symfony2 では、あらかじめ設定されたパターンでリクエスト URL に対してマッチングを行い、URL を処理するコードへリクエストを引渡します。
デフォルトでは、この処理に使われるパターン（ルートと呼びます）は ``app/config/routing.yml`` コンフィギュレーションファイルに記述します。

.. code-block:: yaml

    # app/config/routing.yml
    _welcome:
        pattern:  /
        defaults: { _controller: AcmeDemoBundle:Welcome:index }

    _demo:
        resource: "@AcmeDemoBundle/Controller/DemoController.php"
        type:     annotation
        prefix:   /demo

先頭のコメントの後の 3 行は、ユーザーが "``/``" リソースをリクエストした時に実行されるコードを定義しています（ここでは Welcome のページ）。
リクエストがあると、\ ``AcmeDemoBundle:Welcome:index`` コントローラが実行されます。

.. tip::

    Symfony2 Standard Edition では、コンフィギュレーションファイルの形式として `YAML`_ が利用されています。
    Symfony2 では XML、PHP、およびアノテーションによるコンフィギュレーションもネイティブでサポートされています。
    これらのフォーマットは互換性があり、アプリケーションから切り替えて利用することもできます。
    また、初回のリクエスト時にすべてのコンフィギュレーションがキャッシュされますので、アプリケーションのパフォーマンスは利用するコンフィギュレーションのフォーマットには依存しないことに注意してください。

コントローラ
~~~~~~~~~~~~

コントローラは、受け取った\ *リクエスト*\ を処理し、\ *レスポンス*\ （通常は HTML コード）を返します。
``$_GET`` や ``header()`` のような PHP のグローバル変数や関数を使う代わりに、Symfony ではこれらの HTTP メッセージを管理するオブジェクト :class:`Symfony\\Component\\HttpFoundation\\Request` や :class:`Symfony\\Component\\HttpFoundation\\Response` を使います。
リクエストに基づいて、手作業でレスポンスを作成する最も単純なコントローラは、次のようになります。

::

    use Symfony\Component\HttpFoundation\Response;

    $name = $request->query->get('name');

    return new Response('Hello '.$name, 200, array('Content-Type' => 'text/plain'));

.. note::

    シンプルなコンセプトの裏には、強力なポテンシャルがあります。
    Symfony2 が HTTP をラップしている方法、および HTTP をシンプルかつ強力に扱えるようになる理由について知りたい方は、\ ":doc:`/book/http_fundamentals`" の章をお読みください。

ルーティングコンフィギュレーションに設定された ``_controller`` の値を元に、実行するコントローラが決定されます（ここでは ``AcmeDemoBundle:Welcome:index``\ ）。
ここで設定する文字列は、コントローラの\ *論理名*\ で、今回の場合は ``Acme\DemoBundle\Controller\WelcomeController`` クラスの ``indexAction`` メソッドを参照しています。

::

    // src/Acme/DemoBundle/Controller/WelcomeController.php
    namespace Acme\DemoBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class WelcomeController extends Controller
    {
        public function indexAction()
        {
            return $this->render('AcmeDemoBundle:Welcome:index.html.twig');
        }
    }

.. tip::

    ``_controller`` の参照先には
    ``Acme\DemoBundle\Controller\WelcomeController::indexAction`` という値も使用できますが、論理名を使うことでより簡潔で柔軟になります。

コントローラクラスは、組み込みの ``Controller`` クラスを継承します。
このクラスには、テンプレート (``AcmeDemoBundle:Welcome:index.html.twig``) を読み込んでレンダリングする :method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::render` メソッドのような便利なショートカットメソッドが定義されています。
メソッドの戻り値は Response オブジェクトで、レンダリング結果が含まれています。
必要であれば、ユーザーのブラウザへ送信される前に Response の内容を変更できます。

::

    public function indexAction()
    {
        $response = $this->render('AcmeDemoBundle:Welcome:index.txt.twig');
        $response->headers->set('Content-Type', 'text/plain');

        return $response;
    }

.. tip::

    ``Controller`` 基底クラスの継承は必須ではありません。
    実際、プレーンな PHP 関数や PHP のクロージャをコントローラにすることもできます。
    Symfony2 のコントローラに関する詳細は、ドキュメントの ":doc:`The Controller</book/controller>`" の章を参照してください。

テンプレート名 ``AcmeDemoBundle:Welcome:index.html.twig`` もテンプレートの\ *論理名*\ で、\ ``src/Acme/DemoBundle/Resources/views/Welcome/index.html.twig`` ファイルを参照しています。
このように論理名を使うことの有用性は、後のバンドルの節で説明します。

それでは再度、ルーティングコンフィギュレーションの最後の方を見てみましょう。

.. code-block:: yaml

    # app/config/routing.yml
    _demo:
        resource: "@AcmeDemoBundle/Controller/DemoController.php"
        type:     annotation
        prefix:   /demo

Symfony2 では、YAML、XML、PHP での設定や、PHP のアノテーションに埋め込まれた設定など、さまざまなリソースからルーティングの情報を読み込むことができます。
ここでは、読み込むリソースの\ *論理名*\ は ``@AcmeDemoBundle/Controller/DemoController.php`` で、\ ``src/Acme/DemoBundle/Controller/DemoController.php`` ファイルを参照しています。
このファイルには、アクションメソッドのアノテーションとしてルートが定義されています。

::

    // src/Acme/DemoBundle/Controller/DemoController.php
    class DemoController extends Controller
    {
        /**
         * @extra:Route("/hello/{name}", name="_demo_hello")
         * @extra:Template()
         */
        public function helloAction($name)
        {
            return array('name' => $name);
        }

        // ...
    }

``@extra:Route()`` アノテーションにより ``/hello/{name}`` というパターンの新しいルートが定義され、このルートにマッチした場合は ``helloAction`` メソッドが実行されることになります。
``{name}`` のように波括弧で囲まれた文字列をプレースホルダと呼びます。
すでに見てきたように、この部分の値はメソッドの ``$name`` という引数に渡されます。

.. note::

    アノテーションは PHP でネイティブにサポートされている機能ではありませんが、Symfony2 ではさまざまな場所でアノテーションを使えます。
    アノテーションを使うと、フレームワークの振舞を簡単に設定できるだけでなく、コードのすぐ近くに設定を記述しておけるので便利です。

アクションのコードを良く見てみましょう。先ほどの例ではテンプレートをレンダリングしていましたが、このアクションでは単にパラメータの配列を返しています。
``@extra:Template()`` アノテーションによりレンダリングするテンプレートが決定され、戻り値の配列のそれぞれの値がテンプレートに引き渡されます。
レンダリングされるテンプレートの名前は、コントローラの名前に基づいて決定されます。
この例の場合、\ ``AcmeDemoBundle:Demo:hello.html.twig`` テンプレートがレンダリングされます（\ ``src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig`` ファイルに対応しています）。

.. tip::

    ``@extra:Route()`` アノテーションと ``@extra:Template()`` アノテーションには、このチュートリアルで説明したものよりもさらに多くの機能があります。
    詳細は、\ "`annotations in controllers`_" ドキュメントを参照してください。

テンプレート
~~~~~~~~~~~~

さきほどのコントローラでは、\ ``src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig`` テンプレート（論理名では ``AcmeDemoBundle:Demo:hello.html.twig``\ ）がレンダリングされます。

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {% block title "Hello " ~ name %}

    {% block content %}
        <h1>Hello {{ name }}!</h1>
    {% endblock %}

Symfony2 は、デフォルトのテンプレートエンジンとして `Twig`_ を利用しますが、PHP テンプレートも使えます。
次の章では、Symfony2 でテンプレートがどのように処理されるのかを解説します。

バンドル
~~~~~~~~

これまでの説明で\ :term:`バンドル`\ という単語が多く使われているのを不思議に思われたかもしれません。
アプリケーション用に記述するコードは、すべてバンドルの中にまとめられます。
Symfony2 におけるバンドルとは、ブログやフォーラムといった 1 つのフィーチャーを実装する PHP ファイル、スタイルシート、JavaScript、画像などが構造化されまとめられたもので、簡単に他の開発者と共有できます。
このチュートリアルでは、\ ``AcmeDemoBundle`` という名前のバンドルを扱います。
バンドルについてさらに学習したい方は、チュートリアルの最後の章をお読みください。

環境を切り替える
----------------

Symfony2 の動作について少し理解が進んだでしょうか。
それでは Web ブラウザに表示されたページの一番下にある、Symfony2 のロゴのある小さなバーを見てください。
このバーは "Web デバッグツールバー" と呼ばれ、開発時にとても役に立ちます。
ただし、このバーに表示されているのは情報のほんの一部だけです。
バーに表示されている 16 進数の数字のリンクをクリックすると、プロファイラーと呼ばれる、さらに便利な Symfony2 のデバッグツールが表示されます。

当然ですが、アプリケーションを運用環境へデプロイしたら、このようなデバッグツールが表示されては困ります。
このような理由から、Symfony2 では運用環境に最適化されたもう 1 つのフロントコントローラが提供されており、\ ``web/`` ディレクトリの (``app.php``) ファイルです。

.. code-block:: text

    http://localhost/Symfony/web/app.php/demo/hello/Fabien

Apache で ``mod_rewrite`` を有効にしている場合は、URL の ``app.php`` の部分を省略できます。

.. code-block:: text

    http://localhost/Symfony/web/demo/hello/Fabien

最後に、これだけで十分というわけではありませんが、セキュリティの観点、および URL を分かりやすくする目的で、運用サーバーでは Web サーバーのドキュメントルートディレクトリが ``web/`` ディレクトリを指すように設定してください。

.. code-block:: text

    http://localhost/demo/hello/Fabien

アプリケーションの応答を高速化するために、Symfony2 ではキャッシュファイルを ``app/cache/`` ディレクトリに作成します。
開発用の環境 (``app_dev.php``) の場合、コードやコンフィギュレーションを変更したら、自動的にキャッシュがクリアされます。
しかし、パフォーマンスが重要である運用環境 (``app.php``) の場合は、キャッシュは自動的にはクリアされません。
このような理由から、開発を行なっている場合は常に開発用の環境を利用するようにしてください。

特定のアプリケーションの複数の\ :term:`環境<environment>`\ は、コンフィギュレーションのみが異なります。
コンフィギュレーションは、別のコンフィギュレーションを継承することもできます。

.. code-block:: yaml

    # app/config/config_dev.yml
    imports:
        - { resource: config.yml }

    web_profiler:
        toolbar: true
        intercept_redirects: false

``dev`` 環境（\ ``config_dev.yml`` で定義される）は、グローバルな ``config.yml`` ファイルを継承し、Web デバッグツールバーの有効化などの拡張が行われいます。

まとめ
------

お疲れさまでした！
初めての Symfony2 のコードはいかがでしたか？
それほど難しくはありませんよね。
学ぶことはまだまだたくさんありますが、Symfony2 を使ってより良く、早く Web サイトを実装できることを学びました。
Symfony2 についてさらに学んでみよう！という方は、次の "ビュー" の章へ進んでみましょう！

.. _Symfony2 Standard Edition:      http://symfony.com/download
.. _Symfony in 5 minutes:           http://symfony.com/symfony-in-five-minutes
.. _関心事の分離:                   http://en.wikipedia.org/wiki/Separation_of_concerns
.. _YAML:                           http://www.yaml.org/
.. _annotations in controllers:     http://bundles.symfony-reloaded.org/frameworkextrabundle/
.. _Twig:                           http://www.twig-project.org/
