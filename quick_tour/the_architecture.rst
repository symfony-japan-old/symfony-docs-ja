.. note::

    * 対象バージョン：2.3以降
    * 翻訳更新日：2013/11/23

アーキテクチャ
==============

最初の 3 つの章を読み終えてこの章へたどり着いたみなさん、お疲れ様でした。
ここまでで学んだ内容は、すぐにあなたの役に立ちます。
ただし、この 3 つの章では、フレームワークのアーキテクチャーについて深くは学びませんでした。
Symfony2 のアーキテクチャーは、さまざまなフレームワークの中でも際立っています。
この章では Symfony2 のアーキテクチャーについて学びます。

ディレクトリ構造について
------------------------

Symfony2 :term:`アプリケーション`\ のディレクトリ構造に制限はほとんどなく柔軟ですが、\ *Standard Edition* ディストリビューションでは、アプリケーションで典型的に使用するディレクトリの推奨構成が採用されています。

* ``app/``:    アプリケーションのコンフィギュレーション
* ``src/``:    プロジェクトの PHP コード
* ``vendor/``: サードパーティの依存ライブラリ
* ``web/``:    Web ルートディレクトリ

``web/`` ディレクトリ
~~~~~~~~~~~~~~~~~~~~~

web ディレクトリは、画像や JavaScript、スタイルシートなどの Web に公開する静的ファイルの基点となるディレクトリです。
また、各\ :term:`フロントコントローラ`\ ファイルもこのディレクトリに配置されます。

::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

カーネルは最初に ``bootstrap.php.cache`` ファイルを読み込みます。
このファイルでは、フレームワークの初期化処理とオートローダの登録処理が行われます。

フロントコントローラのファイルである ``app.php`` は、カーネルクラスとして ``AppKernel`` を使い、アプリケーションを起動します。

``app/`` ディレクトリ
~~~~~~~~~~~~~~~~~~~~~

``AppKernel`` クラスは、アプリケーションコンフィギュレーションの中心となるエントリポイントで、ファイルは ``app/`` ディレクトリにあります。

このクラスには、次の 2 つのメソッドを実装する必要があります。

* ``registerBundles()`` メソッドは、アプリケーションの実行に必要なすべてのバンドルの配列を返すようにします。

* ``registerContainerConfiguration()`` メソッドは、アプリケーションコンフィギュレーションを読み込むようにします。

オートロードは `Composer`_ により処理されるので、特に何もしなくても任意の PHP クラスをアプリケーションのコードから利用できます。拡張が必要な場合は ``app/autoload.php`` スクリプトを修正します。すべての依存パッケージは ``vendor/`` ディレクトリ配下に格納されますが、ディレクトリは単なる規約です。サーバーのグローバルなディレクトリや、プロジェクトローカルのディレクトリ等に変更できます。

.. note::

    Composer のオートローダーについての詳細は、\ `Composer のオートローダーについて`_ のページを参照してください。
    Symfony にもオートロード用のコンポーネントがあります: ":doc:`/components/class_loader/class_loader`"

バンドルシステムについて
------------------------

この節では、Symfony2 の強力な機能の 1 つである\ :term:`バンドル`\ システムについて紹介します。

バンドルは、他のソフトウェアでプラグインと呼ばれているものに似ています。
なぜ\ *プラグイン*\ と呼ばず\ *バンドル*\ と呼ぶのでしょうか。
それは、Symfony2 ではフレームワークのコア機能から、開発者が記述するアプリケーションコードまで、\ *すべて*\ がバンドルだからです。
Symfony2 では、バンドルは第一級オブジェクトです。
バンドルの柔軟性により、よく使う機能が実装されパッケージングされたサードパーティ製のバンドルを自分のアプリケーションで使ったり、自分のバンドルを配布したりできます。
アプリケーションで有効にする機能を選択したり、好きな方法で最適化することも簡単です。
最終的には、作成するアプリケーションコードはコアフレームワークと同じくらい重要になっていきます。

バンドルを登録する
~~~~~~~~~~~~~~~~~~

アプリケーションは、\ ``AppKernel`` クラスの ``registerBundles()`` メソッドで定義されたバンドルで構成されます。
各バンドルはディレクトリになっており、バンドル自身を表す ``Bundle`` クラスが 1 つあります。

::

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

チュートリアルで見てきた ``AcmeDemoBundle`` バンドル以外に、\ ``FrameworkBundle``\ 、\ ``DoctrineBundle``\ 、\ ``SwiftmailerBundle``\ 、\ ``AsseticBundle`` といったバンドルがカーネルに登録されていることが分かります。
これらはすべて、フレームワークのコア機能の一部です。

バンドルのコンフィギュレーション
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

各バンドルは、YAML、XML、PHP などの形式で記述されたコンフィギュレーションファイルでカスタマイズできます。
デフォルトのコンフィギュレーションファイルの中身を見てみましょう。

.. code-block:: yaml

    # app/config/config.yml
    imports:
        - { resource: parameters.yml }
        - { resource: security.yml }

    framework:
        #esi:             ~
        #translator:      { fallback: "%locale%" }
        secret:          "%secret%"
        router:
            resource: "%kernel.root_dir%/config/routing.yml"
            strict_requirements: "%kernel.debug%"
        form:            true
        csrf_protection: true
        validation:      { enable_annotations: true }
        templating:      { engines: ['twig'] } #assets_version: SomeVersionScheme
        default_locale:  "%locale%"
        trusted_proxies: ~
        session:         ~

    # Twig Configuration
    twig:
        debug:            "%kernel.debug%"
        strict_variables: "%kernel.debug%"

    # Assetic Configuration
    assetic:
        debug:          "%kernel.debug%"
        use_controller: false
        bundles:        [ ]
        #java: /usr/bin/java
        filters:
            cssrewrite: ~
            #closure:
            #    jar: "%kernel.root_dir%/Resources/java/compiler.jar"
            #yui_css:
            #    jar: "%kernel.root_dir%/Resources/java/yuicompressor-2.4.7.jar"

    # Doctrine Configuration
    doctrine:
        dbal:
            driver:   "%database_driver%"
            host:     "%database_host%"
            port:     "%database_port%"
            dbname:   "%database_name%"
            user:     "%database_user%"
            password: "%database_password%"
            charset:  UTF8

        orm:
            auto_generate_proxy_classes: "%kernel.debug%"
            auto_mapping: true

    # Swift Mailer Configuration
    swiftmailer:
        transport: "%mailer_transport%"
        host:      "%mailer_host%"
        username:  "%mailer_user%"
        password:  "%mailer_password%"
        spool:     { type: memory }

``framework`` などの各エントリは、特定のバンドルのコンフィギュレーションを定義しています。
たとえば、\ ``framework`` エントリは ``FrameworkBundle`` のコンフィギュレーション、\ ``swiftmailer`` エントリは ``SwiftmailerBundle`` のコンフィギュレーションとなっています。

各\ :term:`環境` 向けのコンフィギュレーションファイルを用意することで、デフォルトのコンフィギュレーションを上書きできます。
たとえば ``dev`` 環境では ``config_dev.yml`` ファイルが読み込まれます。
このファイルではメインのコンフィギュレーションファイル（たとえば\ ``config.yml``\ ）を読み込み、その後デバッギングツール用の設定をいくつか追加します。

.. code-block:: yaml

    # app/config/config_dev.yml
    imports:
        - { resource: config.yml }

    framework:
        router:   { resource: "%kernel.root_dir%/config/routing_dev.yml" }
        profiler: { only_exceptions: false }

    web_profiler:
        toolbar: true
        intercept_redirects: false

    monolog:
        handlers:
            main:
                type:  stream
                path:  "%kernel.logs_dir%/%kernel.environment%.log"
                level: debug
            firephp:
                type:  firephp
                level: info

    assetic:
        use_controller: true

バンドルを拡張する
~~~~~~~~~~~~~~~~~~

バンドルはコードの整理方法や設定方法を提供するだけでなく、他のバンドルを拡張することもできます。
バンドルを継承すると、既存のバンドルの機能をオーバーライドしてコントローラやテンプレート、その他バンドルに含まれる任意のファイルをカスタマイズできます。
このようにバンドルを継承する場合、リソースに論理名 (\ ``@AcmeDemoBundle/Controller/SecuredController.php`` など) を使うと、リソースが実際に格納されている場所を抽象化して扱えるため便利です。

論理ファイル名
..............

バンドルにあるファイルを参照したい場合、\ ``@BUNDLE_NAME/path/to/file`` という記法を使います。
Symfony2 により、\ ``@BUNDLE_NAME`` はバンドルの実際のパスに置き換えられます。
たとえば、\ ``@AcmeDemoBundle/Controller/DemoController.php`` という論理パスの場合、\ ``AcmeDemoBundle`` バンドルのパスは Symfony で管理されているため、\ ``src/Acme/DemoBundle/Controller/DemoController.php`` というパスに変換されます。

論理コントローラ名
..................

コントローラを参照する場合、\ ``BUNDLE_NAME:CONTROLLER_NAME:ACTION_NAME`` という記法でアクションメソッドを指定します。
たとえば、\ ``AcmeDemoBundle:Welcome:index`` の場合は\ ``Acme\DemoBundle\Controller\WelcomeController`` クラスの ``indexAction`` メソッドにマップされます。

論理テンプレート名
..................

テンプレートを参照する場合、\ ``AcmeDemoBundle:Welcome:index.html.twig`` という論理名は ``src/Acme/DemoBundle/Resources/views/Welcome/index.html.twig`` というファイルのパスに変換されます。
テンプレートに関する面白い機能としては、必ずしもファイルシステムに保存されている必要はないというものがあります。たとえばデータベースのテーブルに保存するように簡単に変更できます。

バンドルを拡張する
..................

これらの規約に従うことで、\ :doc:`バンドルの継承 </cookbook/bundles/inheritance>`\ の機能を使ってファイル、コントローラー、テンプレートを "上書き" できるようになります。
たとえば、新しい ``AcmeNewBundle`` という名前のバンドルが ``AcmeDemoBundle`` を継承している場合、Symfony により、まず最初に ``AcmeNewBundle`` の中にある ``AcmeDemoBundle:Welcome:index`` コントローラが検索され、次に ``AcmeDemoBundle`` のコントローラが検索されます。

Symfony2 の柔軟性が少しずつ分かってきたでしょうか。
アプリケーション間でバンドルを共有したり、プロジェクトローカルやサーバー上のグローバルな位置に配置するといったことも自由にできます。

.. _using-vendors:

vendor ディレクトリの使い方
---------------------------

構築するアプリケーションがサードパーティのライブラリに依存している場合もあるでしょう。
このようなライブラリは、\ ``vendor/`` ディレクトリへ配置することをおすすめします。
このディレクトリには、SwiftMailer ライブラリ、Doctrine ORM、Twig テンプレートエンジン、および他のサードパーティライブラリやバンドルといった Symfony2 のライブラリがすでに配置されています。

キャッシュとログについて
------------------------

Symfony2 はフルスタックのフレームワークの中で最も高速なものの 1 つでしょう。
しかし、膨大な YAML や XML のファイルをリクエストの度にパースして解析していたら、このような速度は得られません。
高速な応答にとって重要な要素の 1 つに、キャッシュシステムがあります。
アプリケーションコンフィギュレーションは最初のリクエストの時にのみパースされ、プレーンな PHP コードにコンパイルされて ``app/cache/`` ディレクトリへ保存されます。
開発環境の場合は、コンフィギュレーションの変更を Symfony2 が検知してキャッシュのクリアを行います。
しかし運用環境では、コードやコンフィギュレーションを更新した後にキャッシュをクリアすることは開発者の責務となっています。

Web アプリケーションを構築していると、何かがおかしくなってしまう場合があります。
``app/logs/`` ディレクトリにあるログファイルを見ると、リクエストに関するすべての情報を確認でき、問題の原因を素早く見つけるのに役立ちます。

コマンドラインインターフェイス
------------------------------

Symfony2 アプリケーションにはコマンドラインインターフェイス用のツール（\ ``app/console``\ ）が組み込まれており、アプリケーションのメンテナンスに役立ちます。
また、何度も実行するようなタスクを自動化するコマンドを使うと、生産性が大きく向上します。

引数を指定せずに実行すると、その時点で利用可能なコマンドの一覧が表示されます。

.. code-block:: bash

    $ php app/console

``--help`` オプションを指定して実行すると、コマンドの使用方法が表示されます。

.. code-block:: bash

    $ php app/console router:debug --help

まとめ
------

この章の内容を読み終えたので、いろいろ構成を変更して自分の使いやすいように Symfony2 を設定することもできるでしょう。
Symfony2 は、自分のやり方に合わせられるように設計されています。
ですので、ディレクトリ名を変更したり移動させたりして、自分に合うようにしてみてください。

これでクイックツアーはすべて完了です。
Symfony2 マスターになるためには、テストの方法やメールの送信方法など、まだ多くのことを学ぶ必要があります。
さらに学習したい方は、\ :doc:`/book/index` から気になるトピックへ進んでください。

.. _standards:  http://symfony.com/PSR0
.. _convention: http://pear.php.net/
.. _Composer:   http://getcomposer.org
.. _`Composer のオートローダーについて`: http://getcomposer.org/doc/01-basic-usage.md#autoloading

.. 2013/11/23 hidenorigoto d11327b2c28ebb71b9cdc1b5cf5879183905b3ad
