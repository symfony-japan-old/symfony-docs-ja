.. index::
   pair: Assetic; Configuration reference

.. note::
    * 対象バージョン：2.3 (2.1以降)
    * 翻訳更新日：2013/11/23

AsseticBundle 設定("assetic")
==================

すべての設定値の説明および初期値
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. configuration-block::

    .. code-block:: yaml

        assetic:
            debug:                "%kernel.debug%"
            use_controller:
                enabled:              "%kernel.debug%"
                profiler:             false
            read_from:            "%kernel.root_dir%/../web"
            write_to:             "%assetic.read_from%"
            java:                 /usr/bin/java
            node:                 /usr/bin/node
            ruby:                 /usr/bin/ruby
            sass:                 /usr/bin/sass
            # 要素の名前がついたキーと値のペアをいくつでも追加可能
            variables:
                some_name:                 []
            bundles:

                # 初期値 (全登録済みバンドル):
                - FrameworkBundle
                - SecurityBundle
                - TwigBundle
                - MonologBundle
                - SwiftmailerBundle
                - DoctrineBundle
                - AsseticBundle
                - ...
            assets:
                # 名前にassetがつく配列 (例: some_asset, some_other_asset)(?)
                some_asset:
                    inputs:               []
                    filters:              []
                    options:
                        #オプションと値の連想配列
                        some_option_name: []
            filters:

                # 名前にfilterがつく配列 (例: some_filter, some_other_filter)
                some_filter:                 []

            twig:
                functions:
                    # 名前にfunctionがつく配列 (例: some_function, some_other_function)
                    some_function:                 []

    .. code-block:: xml

        <assetic:config
            debug="%kernel.debug%"
            use-controller="%kernel.debug%"
            read-from="%kernel.root_dir%/../web"
            write-to="%assetic.read_from%"
            java="/usr/bin/java"
            node="/usr/bin/node"
            sass="/usr/bin/sass"
        >
            <!-- 初期値 (全登録済みバンドル) -->
            <assetic:bundle>FrameworkBundle</assetic:bundle>
            <assetic:bundle>SecurityBundle</assetic:bundle>
            <assetic:bundle>TwigBundle</assetic:bundle>
            <assetic:bundle>MonologBundle</assetic:bundle>
            <assetic:bundle>SwiftmailerBundle</assetic:bundle>
            <assetic:bundle>DoctrineBundle</assetic:bundle>
            <assetic:bundle>AsseticBundle</assetic:bundle>
            <assetic:bundle>...</assetic:bundle>

            <assetic:asset>
                <!-- プロトタイプ -->
                <assetic:name>
                    <assetic:input />

                    <assetic:filter />

                    <assetic:option>
                        <!-- プロトタイプ -->
                        <assetic:name />
                    </assetic:option>
                </assetic:name>
            </assetic:asset>

            <assetic:filter>
                <!-- プロトタイプ -->
                <assetic:name />
            </assetic:filter>

            <assetic:twig>
                <assetic:functions>
                    <!-- プロトタイプ -->
                    <assetic:name />
                </assetic:functions>
            </assetic:twig>

        </assetic:config>

.. 2011/07/03 jptomo 8ac37d1c76f2d6ad73fd1d24b73ee159542c719d
.. 2013/11/23 sotarof 74b92f5a6e6085b8718c727185d833d841061cf3
