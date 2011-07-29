.. translation finished: 30th July 2011
   id: chobie
   hash: c12fa7ea5bdf82b809ea0eba55fdb19a2aa67637

YUI Compressorを使ってJavascriptやStylesheetのサイズを圧縮するには?
===================================================================

Yahoo! が提供する`YUI Compressor`_ というとびきり優れたツールを使う事でJavascriptやStyleSheetのサイズを圧縮することが可能となります。
彼らはこれで素早くAssetデータを転送しているのです。Symfony2に付属しているAsseticを使うことでこのツールを簡単に扱うことが出来ます。

YUI CompressorのJARファイルのダウンロード
-----------------------------------------

YUI CompressorはJARファイルとして配布されています。 Yahooのサイトからから `Download the JAR`_  して
``app/Resources/java/yuicompressor.jar`` に配置しましょう。

YUI Filterの設定
----------------

それではJavascriptのサイズを小さくするためのフィルタと、Stylesheetのサイズを
小さくするための二つのAsseticフィルタを設定しましょう。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                yui_css:
                    jar: "%kernel.root_dir%/Resources/java/yuicompressor.jar"
                yui_js:
                    jar: "%kernel.root_dir%/Resources/java/yuicompressor.jar"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="yui_css"
                jar="%kernel.root_dir%/Resources/java/yuicompressor.jar" />
            <assetic:filter
                name="yui_js"
                jar="%kernel.root_dir%/Resources/java/yuicompressor.jar" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'yui_css' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
                'yui_js' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
            ),
        ));

これで新しく追加した二つのAsseticフィルタに``yui_css`` と ``yui_js`` という名称で
アプリケーションでフィルタを適用できるようになりました。
これら二つの設定はそれぞれYUI CompressorでJavascriptとStylesheetのデータサイズを圧縮する際に使われます。

実際にAseetsファイルのデータを圧縮する
--------------------------------------

YUI Compressorの設定は終わりました。しかし、これらのフィルタを適用しない限り実際には何の処理も行われません。
あなたのAssetsファイルがViewレイヤにあるならば下記のサンプルのようににテンプレートファイルに記述することでフィルタが適用されます。

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' filter='yui_js' %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array('yui_js')) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. note::

    上述の例はあなたが既に ``AcmeFooBundle`` というBundleを設定しており、Javascript
    ファイルが ``Resources/public/js`` に保存されていると仮定しています。
    しかし、これは特別重要な事ではありません。Javascirptファイルはどこからでも参照することが出来るからです。

assetタグに追記された ``yui_js`` というフィルタをつけることで、サイズを削減したJavascriptが転送されるようになり、
これでAssetファイルの転送時間が短くなります。これはstylesheetに関しても同様です。

.. configuration-block::

    .. code-block:: html+jinja

        {% stylesheets '@AcmeFooBundle/Resources/public/css/*' filter='yui_css' %}
        <link rel="stylesheet" type="text/css" media="screen" href="{{ asset_url }}" />
        {% endstylesheets %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->stylesheets(
            array('@AcmeFooBundle/Resources/public/css/*'),
            array('yui_css')) as $url): ?>
        <link rel="stylesheet" type="text/css" media="screen" href="<?php echo $view->escape($url) ?>" />
        <?php endforeach; ?>

デバッグ時にサイズ圧縮機能を無効にする
--------------------------------------

サイズ圧縮されたJavascriptやStylesheetは非常に読みづらく、言うまでもなくデバッグ時には無効にしたいと思います。
Asseticはデバッグモード時にフィルタを無効にできる機能も備えています。デバッグ時にフィルタを無効
にするにはテンプレートに記述するフィルタ名の前に ``?`` をつけます。これはAsseticにデバッグモードが
無効な時にだけフィルタを適用するように指示します。

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' filter='?yui_js' %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array('?yui_js')) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. _`YUI Compressor`: http://developer.yahoo.com/yui/compressor/
.. _`Download the JAR`: http://yuilibrary.com/downloads/#yuicompressor