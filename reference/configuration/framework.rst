.. index::
    single: Configuration reference; Framework

.. note::

    * 対象バージョン：2.3+
    * 翻訳更新日：2014/08/01


FrameworkBundle 設定 ("framework")
========================================

このリファレンス文書はまだ未完成です。本来正確でなければならいないのですが、
すべてのオプションはまだ完全には対応していません。

``FrameworkBundle`` はベースとなるフレームワーク機能のほとんどが含まれおり
アプリケーションのコンフィグレーション内の ``framework`` キー配下で構成することができます。
これを含む設定の関連に、sessions、transaction、forms、validation、routingなどがあります。

設定
-------------

* `secret`_
* `http_method_override`_
* `ide`_
* `test`_
* `trusted_proxies`_
* `csrf_protection`_
    * enabled
    * field_name (非推奨)
* `form`_
    * enabled
    * csrf_protection
        * enabled
        * field_name
* `session`_
    * `name`_
    * `cookie_lifetime`_
    * `cookie_path`_
    * `cookie_domain`_
    * `cookie_secure`_
    * `cookie_httponly`_
    * `gc_divisor`_
    * `gc_probability`_
    * `gc_maxlifetime`_
    * `save_path`_
* `serializer`_
    * :ref:`enabled<serializer.enabled>`
* `templating`_
    * `assets_base_urls`_
    * `assets_version`_
    * `assets_version_format`_
* `profiler`_
    * `collect`_
    * :ref:`enabled <profiler.enabled>`

secret
~~~~~~

**データ型**: ``string`` **必須**

これはアプリケーションで固有にしなければならない文字列です。

実際には、CSRFトークンを生成するために使われていますが、ユニークな文字列を持つことが
有用な別のコンテキストで使用することができます。それは ``kernel.secret`` という名前の
サービスコンテナにすることができます。

.. _configuration-framework-http_method_override:

http_method_override
~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    ``http_method_override`` オプションは Symfony 2.3 から導入されました。

**データ型**: ``Boolean`` **デフォルト**: ``true``

POSTリクエストにおいて、リクエストパラメータ ``_method`` が、意図した HTTP メソッドとして使われるかどうかを決定します。
有効にすると、
:method:`Request::enableHttpMethodParameterOverride <Symfony\\Component\\HttpFoundation\\Request::enableHttpMethodParameterOverride>`
が自動的に呼び出されます。これは、\ ``kernel.http_method_override`` という名前のサービスコンテナパラメータになりました。
詳細については、\ :doc:`/cookbook/routing/method_parameters` を参照して下さい。

ide
~~~

**データ型**: ``string`` **デフォルト**: ``null``

もしTextMate か Mac VimのようなIDEを使用しているなら、Symfonyは
例外メッセージ内のファイルパスのすべてをリンクに変えることができ、
IDEでそのファイルを開きます。

Symfony はいくつかの有名な IDE のために事前設定した URL を持っており、
次のキーを使うことでそれらを設定できます。:

* ``textmate``
* ``macvim``
* ``emacs``
* ``sublime``

.. versionadded:: 2.3.14
    エディター の ``emacs`` と ``sublime`` が Symfony 2.3.14 で導入されました。

さらにカスタムの url 文字列を指定することもできます。これを行う場合は
エスケープのためすべてのパーセント記号(``%``)を二重にする必要があります。例えば、
`PhpStormOpener`_ をインストールし ``PhpStorm`` を使用している場合は次のようになります。:

.. configuration-block::

    .. code-block:: yaml

        framework:
            ide: "pstorm://%%f:%%l"

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/service"
            xmlns:framework="http://symfony.com/schema/dic/symfony">

            <framework:config ide="pstorm://%%f:%%l" />

        </container>

    .. code-block:: php

        $container->loadFromExtension('framework', array(
            'ide' => 'pstorm://%%f:%%l',
        ));

もちろん、すべての開発者が別々のIDEを使っているので、システムレベルで設定するとよいでしょう。
``php.ini`` の ``xdebug.file_link_format`` に url の文字列を設定することによって行うことができます。
この設定値がセットされている場合、\ ``ide`` オプションは無視されます。

.. _reference-framework-test:

test
~~~~

**データ型**: ``Boolean``

この設定パラメーターが存在する（且つ ``false`` でない）場合、
あなたのアプリケーションのテストに関連するサービス（例えば ``test.client``）がロードされます。
この設定は、テスト環境で存在すべきです（通常は ``app/config/config_test.yml`` 経由で）。
詳細については、以下を参照してください。\ :doc:`/book/testing` 。

.. _reference-framework-trusted-proxies:

trusted_proxies
~~~~~~~~~~~~~~~

**データ型**: ``array``

プロキシとして信頼されるべきIPアドレスを設定します。詳細については、
:doc:`/components/http_foundation/trusting_proxies` を参照ください。

.. versionadded:: 2.3
    CIDR 表記のサポートは Symfonyの2.3 で導入されました。サブネット全体（例： ``10.0.0.0/8`` , ``fc00::/7`` ）をホワイトリストに登録することができます。

.. configuration-block::

    .. code-block:: yaml

        framework:
            trusted_proxies:  [192.0.0.1, 10.0.0.0/8]

    .. code-block:: xml

        <framework:config trusted-proxies="192.0.0.1, 10.0.0.0/8">
            <!-- ... -->
        </framework>

    .. code-block:: php

        $container->loadFromExtension('framework', array(
            'trusted_proxies' => array('192.0.0.1', '10.0.0.0/8'),
        ));

.. _reference-framework-form:

form
~~~~

csrf_protection
~~~~~~~~~~~~~~~

session
~~~~~~~

name
....

**データ型**: ``string`` **デフォルト**: ``null``

これはセッション Cookie の名前を指定します。デフォルトでは ``php.ini`` で定義されている ``session.name`` ディレクティブの Cookie 名を使用します。

cookie_lifetime
...............

**データ型**: ``integer`` **デフォルト**: ``null``

これは、セッションの有効期間を秒数で指定します。デフォルトでは ``null`` が使われ、
それは ``php.ini`` の ``session.cookie_lifetime`` の値が使用されることを意味します。
値に ``0`` を設定することは、 Cookie がブラウザを閉じるまで有効であることを意味します。

cookie_path
...........

**データ型**: ``string`` **デフォルト**: ``/``

これは、セッション·クッキーに設定するパスを決定します。デフォルトでは ``/`` を使用します。

cookie_domain
.............

**データ型**: ``string`` **デフォルト**: ``''``

これはセッション Cookie で設定するドメインを決定します。デフォルトでは空白です。
それは、 Cookie の仕様で、 Cookie を作成したサーバのホスト名を意味します。

cookie_secure
.............

**データ型**: ``Boolean`` **デフォルト**: ``false``

セキュアな接続を通じてのみ Cookie を送信できるかどうかを指定します。

cookie_httponly
...............

**データ型**: ``Boolean`` **デフォルト**: ``false``

クッキーに対して、HTTP を通してのみアクセスできるようにします。 つまり、JavaScript のようなスクリプト言語からはアクセスできなくなるということです。
この設定を使用すると、XSS 攻撃によって ID を盗まれる危険性を減らせます。

gc_probability
..............

**データ型**: ``integer`` **デフォルト**: ``1``

これは、セッション開始時の初期化処理でガベージコレクタ（GC）が実行される確率を定義しています。
この確率は、\ ``gc_probability`` / ``gc_divisor`` を用いて算出されます。
例えば、 1/100 は、全リクエストの 1% で GC 処理が開始するチャンスがあることを意味します。

gc_divisor
..........

**データ型**: ``integer`` **デフォルト**: ``100``

`gc_probability`_ を参照してください。

gc_maxlifetime
..............

**データ型**: ``integer`` **デフォルト**: ``1440``

これは、データを「ごみ」とみなした後、消去されるまでの秒数を指定します。
ガベージコレクションは、セッションの開始時に行われ `gc_divisor`_ と `gc_probability`_ に依存します。

save_path
.........

**データ型**: ``string`` **デフォルト**: ``%kernel.cache.dir%/sessions``

これは保存ハンドラに渡される引数を定義します。デフォルトのファイルハンドラを選択した場合、
セッションファイルが作成されるパスになります。
詳細については、 :doc:`/cookbook/session/sessions_directory` を参照ください。

また、値を ``null`` に設定することで、\ ``php.ini`` の ``save_path`` の値をセットすることもできます。:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session:
                save_path: null

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session save-path="null" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'session' => array(
                'save_path' => null,
            ),
        ));

.. _configuration-framework-serializer:

serializer
~~~~~~~~~~

.. _serializer.enabled:

enabled
.......

**データ型**: ``boolean`` **デフォルト**: ``false``

サービスコンテナに ``serializer`` サービスを有効にするかどうか。

詳細については、:doc:`/cookbook/serializer` を参照してください。

templating
~~~~~~~~~~

assets_base_urls
................

**デフォルト**: ``{ http: [], ssl: [] }``

このオプションは、\ ``http`` と ``ssl`` （ ``https`` ）のページにおいて、アセットを参照する基準となる URL を定義します。
単一要素の配列の代わりに文字列を設定することも出来ます。
複数の基準となる URL が提供されている場合、 Symfony2 はアセットのパスを生成するたびにコレクションの中から一つを選択します。

利便性を考慮して、\ ``assets_base_urls`` には直接文字列を設定することも、または、文字列の配列を設定することもできます。
自動的に ``http`` と ``https`` リクエストの基準となる URL のコレクションが作られます。
URLが ``https://`` 、または、\ `protocol-relative`_ （すなわち `//` で始まる）で始まる場合は、両方のコレクションに追加されます。
``http://`` だけで始まる場合、 ``http`` コレクションにのみ追加されます。

.. _ref-framework-assets-version:

assets_version
..............

**データ型**: ``string``

このオプションは、アセットのキャッシュを *削除*  する為に使われます。
表示されたアセットのパスに包括的にクエリパラメータを追加することで行います（例：``/images/logo.png?v2``）。
この機能は、 Assetic でのアセットの表示と同じく、\ ``Twig`` の ``asset`` 関数（またはPHPの同等のもの）で表示したアセットにのみ適用されます。

たとえば、以下のものがあるとします:

.. configuration-block::

    .. code-block:: html+jinja

        <img src="{{ asset('images/logo.png') }}" alt="Symfony!" />

    .. code-block:: php

        <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" alt="Symfony!" />

デフォルトでは、これは ``/images/logo.png`` のように画像のパスを出力します。
ここで ``assets_version`` オプションを有効にしてみます:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating: { engines: ['twig'], assets_version: v2 }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:templating assets-version="v2">
            <framework:engine id="twig" />
        </framework:templating>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            ...,
            'templating'      => array(
                'engines'        => array('twig'),
                'assets_version' => 'v2',
            ),
        ));

今、同じアセットが ``/images/logo.png?V2`` として表示されます。
この機能を使う場合、\ ``assets_version`` の値をクエリパラメータ変更のためにデプロイ前に毎回、手動で増やす *必要があります* 。

（グローバルバージョンを使用しての、例えばここでは ``V2`` の代わりに）アセットごとにバージョン値を設定することもできます。
詳細は :ref:`Versioning by Asset <book-templating-version-by-asset>` を参照ください。

また、 `assets_version_format`_ オプションでクエリー文字列がどのように機能するかも制御することができます。

assets_version_format
.....................

**データ型**: ``string`` **デフォルト**: ``%%s?%%s``

このオプションは、アセットのパスを構築するために、`assets_version`_ オプションとあわせて :phpfunction:`sprintf` のパターンを指定します。
デフォルトのパターンは、クエリ文字列としてアセットのバージョンを追加します。
例えば、``assets_version_format`` に ``%%s?version=%%s`` 、且つ、\ ``assets_version`` に ``5`` を設定している場合、アセットのパスは ``/images/logo.png?version=5`` になります。

.. note::

    フォーマット文字列内のすべてのパーセント記号（ ``%`` ）は、文字をエスケープする為に二重にしてください。
    エスケープしないと、 値は意図せず :ref:`book-service-container-parameters` と解釈される可能性があります。

.. tip::

    一部のCDNはクエリ文字列を使用したキャッシュ削除をサポートしていません。
    そのため、その実際のファイルパスの中にバージョンを入れる必要があります。
    ありがたいことに、\ ``assets_version_format`` はバージョン付きクエリ文字列の生成に限定されるものではありません。

    パターンは、それぞれ第1、及び、第2のパラメータとしてアセットの元のパスとバージョンを受け取ります。
    アセットのパスは一つのパラメータなので、その場で、変更することはできません（例： ``/images/logo-v5.png``）。
    ただし、``version-%%2$s/%%1$s`` パターンを使用してアセットパスの前に付加することが出来ます。
    その結果、\ ``version-5/images/logo.png`` のようなパスになります。

    URL書き換えルールは、アセットを提供する前に、バージョンプレフィックスを無視するために用いることができます。
    別の方法として、デプロイ手順の一部として、適切なバージョンのパスにアセットをコピーして、URLの書き換えは見合わせるということも出来ます。
    古いアセットのバージョンを元のURLでアクセスできる状態で維持したい場合は、後者のオプションは便利です。

profiler
~~~~~~~~

.. _profiler.enabled:

enabled
.......

**デフォルト**: ``dev`` と ``test`` 環境では ``true``

プロファイラは、このキーに ``false`` を設定することで無効にすることができます。

.. versionadded:: 2.3
    ``collect`` オプションはsymfonyの2.3で導入されました。以前は、\ ``profiler.enabled`` が ``false`` のとき、
    プロファイラは実際には有効 *でした* 。しかし、コレクタが無効になっていました。
    現在は、プロファイラやコレクターを独立に制御することができます。

collect
.......

**デフォルト**: ``true``

このオプションは、それが有効になっているときに、プロファイラの動作方法を設定します。
``true`` に設定すると、プロファイラはすべてのリクエストのデータを収集します。
あなただけの、オンデマンドの情報を収集する場合は、\ ``collect`` フラグを ``false`` に設定し、
手でデータコレクタをアクティブにすることで出来ます。::

    $profiler->enable();

全デフォルト設定
-----------------

.. configuration-block::

    .. code-block:: yaml

        framework:
            secret:               ~
            http_method_override: true
            trusted_proxies:      []
            ide:                  ~
            test:                 ~
            default_locale:       en

            csrf_protection:
                enabled:              false
                field_name:           _token # 2.4 から非推奨。 3.0 で削除されます。代わりに form.csrf_protection.field_name を使用してください。

            # form configuration
            form:
                enabled:              false
                csrf_protection:
                    enabled:          true
                    field_name:       ~

            # esi configuration
            esi:
                enabled:              false

            # fragments configuration
            fragments:
                enabled:              false
                path:                 /_fragment

            # profiler configuration
            profiler:
                enabled:              false
                collect:              true
                only_exceptions:      false
                only_master_requests: false
                dsn:                  file:%kernel.cache_dir%/profiler
                username:
                password:
                lifetime:             86400
                matcher:
                    ip:                   ~

                    # use the urldecoded format
                    path:                 ~ # Example: ^/path to resource/
                    service:              ~

            # router configuration
            router:
                resource:             ~ # Required
                type:                 ~
                http_port:            80
                https_port:           443

                # set to true to throw an exception when a parameter does not match the requirements
                # set to false to disable exceptions when a parameter does not match the requirements (and return null instead)
                # set to null to disable parameter checks against requirements
                # 'true' is the preferred configuration in development mode, while 'false' or 'null' might be preferred in production
                strict_requirements:  true

            # session configuration
            session:
                storage_id:           session.storage.native
                handler_id:           session.handler.native_file
                name:                 ~
                cookie_lifetime:      ~
                cookie_path:          ~
                cookie_domain:        ~
                cookie_secure:        ~
                cookie_httponly:      ~
                gc_divisor:           ~
                gc_probability:       ~
                gc_maxlifetime:       ~
                save_path:            %kernel.cache_dir%/sessions

            # serializer configuration
            serializer:
               enabled: false

            # templating configuration
            templating:
                assets_version:       ~
                assets_version_format:  %%s?%%s
                hinclude_default_template:  ~
                form:
                    resources:

                        # Default:
                        - FrameworkBundle:Form
                assets_base_urls:
                    http:                 []
                    ssl:                  []
                cache:                ~
                engines:              # Required

                    # Example:
                    - twig
                loaders:              []
                packages:

                    # Prototype
                    name:
                        version:              ~
                        version_format:       %%s?%%s
                        base_urls:
                            http:                 []
                            ssl:                  []

            # translator configuration
            translator:
                enabled:              false
                fallback:             en

            # validation configuration
            validation:
                enabled:              false
                cache:                ~
                enable_annotations:   false
                translation_domain:   validators

            # annotation configuration
            annotations:
                cache:                file
                file_cache_dir:       %kernel.cache_dir%/annotations
                debug:                %kernel.debug%

.. _`protocol-relative`: http://tools.ietf.org/html/rfc3986#section-4.2
.. _`PhpStormOpener`: https://github.com/pinepain/PhpStormOpener


.. 2012/10/14 chisei a1aaeb5526d0bf09ec6ef9feb14087fa633b1371 (ideまで翻訳)
.. 2014/08/01 yositani2002 00f60a8c622d0ffbf7603a6fe6878a805398cc71
