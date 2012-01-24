.. index::
   single: Environments;

新しい環境を作成して、使いこなす方法
====================================

全てのアプリケーションは、コードとコンフィギュレーションの組み合わせで、コードは機能の動作を決定します。コンフィギュレーションは、使用しているデータベース、何をキャッシュするべきか、ログをどこまで冗長にするか、といったことを定義します。 Symfony2 の "environments" の考えは、複数の異なるコンフィギュレーションで同じコードベースを実行させるという考えになります。例えば、 ``dev`` 環境は、開発をより簡単にしやすくするためのコンフィギュレーションを使用するべきですし、 ``prod`` 環境では、スピードを最適化したコンフィギュレーションを使用するべきです。

.. index::
   single: Environments; Configuration files

異なる環境、異なるコンフィギュレーションファイル
------------------------------------------------

標準的な Symfony2 アプリケーションでは、 ``dev``, ``prod``, ``test`` の３つの環境があります。すでに議論したように、それぞれの "environment" は、異なるコンフィギュレーションで、同じコードを実行することを示しています。それぞれの環境は、独自のコンフィギュレーションファイルをロードしています。 コンフィギュレーションのフォーマットに YAML を使用しているのであれば、次のファイルが使用されます。
:

 * ``dev`` 環境: ``app/config/config_dev.yml``
 * ``prod`` 環境: ``app/config/config_prod.yml``
 * ``test`` 環境: ``app/config/config_test.yml``

これは、 ``AppKernel`` クラスのデフォルトで使用されているシンプルな標準を経由して動作します。

.. code-block:: php

    // app/AppKernel.php
    // ...
    
    class AppKernel extends Kernel
    {
        // ...

        public function registerContainerConfiguration(LoaderInterface $loader)
        {
            $loader->load(__DIR__.'/config/config_'.$this->getEnvironment().'.yml');
        }
    }

上記を見ればわかるように、 Symfony2 がロードされると 与えられた環境(environment) がどのコンフィギュレーションファイルをロードするか決定します。こうすることによって、エレガントに、パワフルに、そして透過的な方法で、複数の環境を実現できるのです。

もちろん、実際には、それぞれの環境は、他の環境と多少しか異なりません。一般的に全ての環境は共通のコンフィギュレーションのベースのほとんどを共有しています。 "dev" コンフィギュレーションファイルを見てみると、それを簡単に透過的に実現する方法を見ることができます。

.. configuration-block::

    .. code-block:: yaml

        imports:
            - { resource: config.yml }

        # ...

    .. code-block:: xml

        <imports>
            <import resource="config.xml" />
        </imports>

        <!-- ... -->

    .. code-block:: php

        $loader->import('config.php');

        // ...

共通のコンフィギュレーションを共有するため、それぞれの環境のコンフィギュレーションがまず最初に ``config.yml`` という元となるコンフィギュレーションファイルをインポートしています。そして残りで、個々のパラメータをオーバーライドすることでデフォルトのコンフィギュレーションを変更します。例えば、デフォルトでは、 ``web_profiler`` は無効になっています。しかし、 ``dev`` コンフィギュレーションファイルでデフォルトの値を変更しているので、このツールバーは、有効になっています。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        imports:
            - { resource: config.yml }

        web_profiler:
            toolbar: true
            # ...

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <imports>
            <import resource="config.xml" />
        </imports>

        <webprofiler:config
            toolbar="true"
            # ...
        />

    .. code-block:: php

        // app/config/config_dev.php
        $loader->import('config.php');

        $container->loadFromExtension('web_profiler', array(
            'toolbar' => true,
            // ..
        ));

.. index::
   single: Environments; Executing different environments

異なる環境でアプリケーションを実行する
--------------------------------------

フロントコントローラからアプリケーションをロードして、それぞれの環境でアプリケーションを実行します。 ``prod`` 環境であれば ``app.php`` で、 ``dev`` 環境であれば ``app_dev.php`` になります。

.. code-block:: text

    http://localhost/app.php      -> *prod* environment
    http://localhost/app_dev.php  -> *dev* environment

.. note::

   上記の URL では、アプリケーションのウェブルートとして、 ``web/`` ディレクトリを使用するようにウェブサーバに設定されていると想定しています。詳細は、 :doc:`Installing Symfony2</book/installation>` を参照してください。

これらのファイルを見てみると、それぞれのフロントコントローラによって環境(environment)が明示的にセットされているのがわかると思います。

.. code-block:: php
   :linenos:

    <?php

    require_once __DIR__.'/../app/bootstrap_cache.php';
    require_once __DIR__.'/../app/AppCache.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppCache(new AppKernel('prod', false));
    $kernel->handle(Request::createFromGlobals())->send();

上記の通り、 ``prod`` キーは、この環境が ``prod`` 環境で動作するように指定しています。 Symfony2 のアプリケーションは、このコードと環境(environment)の文字列を変更すれば、どの環境でも実行が可能になります。

.. note::

   ``test`` 環境はファンクショナルテストから使用でき、フロントコントローラを介してブラウザからは直接アクセスすることはできません。つまり他の環境と違い、 ``app_test.php`` フロントコントローラファイルは存在しません。

.. index::
   single: Configuration; Debug mode

.. sidebar:: *Debug* Mode

    *環境* のトピックとは関係ないですが、上記のフロントコントローラの 8 行目の ``false`` キーは重要です。これは、アプリケーションを "debug mode" で実行すべきかを指定しています。環境に関係なく、 Symfony2 のアプリケーションは、デバッグモードを ``true`` にも ``false`` にも指定して実行できます。このことはアプリケーションの多くのことに影響があります。例えば、エラーを表示すべきかとか、リクエスト毎にキャッシュファイルを動的に作りなおすのか、などです。必須ではありませんが、一般的にデバッグモードは、 ``dev`` と ``test`` 環境のときには ``true`` にセットされており、 ``prod`` 環境では ``false`` にセットされています。

    内部的には、デバッグモードの値は、サービスコンテナ( :doc:`service container </book/service_container>`) 内の ``kernel.debug`` パラメータになります。アプリケーションのコンフィギュレーションファイルの中を見ると、このパラメータが使われているのを見ることができます。 例えば、次のようにこのパラメータを参照して、Doctrine DBAL を使用した際にログ機能を有効にしています。

    .. configuration-block::

        .. code-block:: yaml

            doctrine:
               dbal:
                   logging:  %kernel.debug%
                   # ...

        .. code-block:: xml

            <doctrine:dbal logging="%kernel.debug%" ... />

        .. code-block:: php

            $container->loadFromExtension('doctrine', array(
                'dbal' => array(
                    'logging'  => '%kernel.debug%',
                    // ...
                ),
                // ...
            ));

.. index::
   single: Environments; Creating a new environment

新しく環境を作成する
--------------------

デフォルトでは、 Symfony2 のアプリケーションは、３つの環境があり、ほとんどケースは、これで十分でしょう。しかし、もちろん環境はコンフィギュレーションに対応する文字列以上の何者でもないので、新しい環境もとても簡単に作成できます。

例えば、開発前に、アプリケーションをベンチマークしなければならないとしましょう。アプリケーションをベンチマークする方法として、 Symfony2 の ``web_profiler`` のみを有効にした本番環境に似た設定を使うことができます。そうすれば、 Symfony2 はベンチマークの間アプリケーションに関する情報を記録することができます。

例として ``benchmark`` という環境を新しく作成し、これを実現してみましょう。新しいコンフィギュレーションファイルを作成してください。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_benchmark.yml

        imports:
            - { resource: config_prod.yml }

        framework:
            profiler: { only_exceptions: false }

    .. code-block:: xml

        <!-- app/config/config_benchmark.xml -->

        <imports>
            <import resource="config_prod.xml" />
        </imports>

        <framework:config>
            <framework:profiler only-exceptions="false" />
        </framework:config>

    .. code-block:: php

        // app/config/config_benchmark.php
        
        $loader->import('config_prod.php')

        $container->loadFromExtension('framework', array(
            'profiler' => array('only-exceptions' => false),
        ));

このシンプルな追加のみで、アプリケーションは新しく ``benchmark`` 環境を使用できるようになりました。

この新しいコンフィギュレーションは、 ``prod`` 環境のコンフィギュレーションをインポートし、プロファイラに関する情報を変更しています。つまり、新しい環境は ``prod`` 環境と同じですが、明示的に上書きした部分だけが異なることになります。

ブラウザからアクセス可能な環境を作成したければ、新しくフロントコントローラも作成する必要があります。 ``web/app.php`` ファイルを ``web/app_benchmark.php`` ファイルにコピーして環境(environment)を ``benchmark`` に書き換えてください。

.. code-block:: php

    <?php

    require_once __DIR__.'/../app/bootstrap.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('benchmark', false);
    $kernel->handle(Request::createFromGlobals())->send();

これで次の URL をアクセスすると新しい環境でアクセスえきるようになりました。
::

    http://localhost/app_benchmark.php

.. note::

   ``dev`` のような環境は、一般の公開としてデプロイしたサーバからはアクセスさせてはいけません。なぜならデバッグを目的とした特定の環境は、アプリケーションや動作しているインフラに関する情報がたくさん詰まっているからです。これらの環境がアクセス可能になっていないことを調べるために、フロントコントローラは、コードの上部に次のように指定して、外部 IP アドレスからのアクセスを保護しています。
   
    .. code-block:: php

        if (!in_array(@$_SERVER['REMOTE_ADDR'], array('127.0.0.1', '::1'))) {
            die('You are not allowed to access this file. Check '.basename(__FILE__).' for more information.');
        }

.. index::
   single: Environments; Cache directory

環境とキャッシュディレクトリ
----------------------------

Symfony2 では、多くの方法でキャッシュのアドバンテージを享受できます。アプリケーションのコンフィギュレーション、ルーティングコンフィギュレーション、 Twig テンプレートや、ファイルシステム上のファイルに格納された PHP オブジェクトまでもキャッシュすることが可能です。

デフォルトでは、これらのキャッシュファイルのほとんどは、 ``app/cache`` ディレクトリに格納されます。しかし、各環境のキャッシュは、まとめて次の
以下にキャッシュされます。

.. code-block:: text

    app/cache/dev   - *dev* 環境のキャッシュディレクトリ
    app/cache/prod  - *prod* 環境のキャッシュディレクトリ

デバッグする際に、実際にどう動いているのか理解するためにキャッシュファイルを調査することが便利なときもあります。その際には、デバッグしている環境によるディレクトリの中を探してください(ほとんどの場合、開発やデバッグになりますので ``dev`` になります)。 ``app/cache/dev`` ディレクトリの場合は、次のようになっています。
:

* ``appDevDebugProjectContainer.php`` - キャシュされた "サービスコンテナ" で、キャッシュされたアプリケーションコンフィギュレーションを指します

* ``appdevUrlGenerator.php`` - URL 生成の際のルーティングコンフィギュレーションによって生成された PHP クラス

* ``appdevUrlMatcher.php`` - ルートのマッチングに使われる PHP ファイルで、URL をマッチさせるルートに使用されますので、コンパイルされた正規表現のロジックを見るにはここを参照してください

* ``twig/`` - キャッシュされた全ての Twig のテンプレート


さらに進むには
--------------

詳細は、 :doc:`/cookbook/configuration/external_parameters` を参照してください。

.. 2011/11/03 ganchiku 2bdd4fb99ade82194618c1728a65d3bc098dc8d2

