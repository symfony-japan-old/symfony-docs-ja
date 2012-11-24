.. index::
   single: Assetic; YUI Compressor

YUI Compressor を使って Javascript や Stylesheet のサイズを圧縮するには?
========================================================================

Yahoo! が提供する `YUI Compressor`_ というとびきり優れたツールを使う事で Javascript や StyleSheet のサイズを圧縮することが可能となります。
彼らはこれで素早く Asset データを転送しているのです。 Symfony2 に付属しているAsseticを使うことでこのツールを簡単に扱うことが出来ます。

YUI Compressor の JAR ファイルのダウンロード
-----------------------------------------

YUI Compressor は JAR ファイルとして配布されています。 Yahoo のサイトからから `Download the JAR`_  して
``app/Resources/java/yuicompressor.jar`` に配置しましょう。

YUI Filter の設定
----------------

それでは Javascript のサイズを小さくするためのフィルタと、 Stylesheet のサイズを
小さくするための二つの Assetic フィルタを設定しましょう。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            # java: "/usr/bin/java"
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
            // 'java' => '/usr/bin/java',
            'filters' => array(
                'yui_css' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
                'yui_js' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
            ),
        ));

.. note::

    Windows ユーザは適切な java のパスに修正する必要があります。
    Windows7 x64 bit では、デフォルトのパスは ``C:\Program Files (x86)\Java\jre6\bin\java.exe`` になります。

これで新しく追加した二つの Assetic フィルタに ``yui_css`` と ``yui_js`` という名称で
アプリケーションでフィルタを適用できるようになりました。
これら二つの設定はそれぞれ YUI Compressor で Javascript と Stylesheet のデータサイズを圧縮する際に使われます。

実際に Aseets ファイルのデータを圧縮する
--------------------------------------

YUI Compressor の設定は終わりました。しかし、これらのフィルタを適用しない限り実際には何の処理も行われません。
あなたの Assets ファイルがViewレイヤにあるならば下記のサンプルのようににテンプレートファイルに記述することでフィルタが適用されます。

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

    上述の例はあなたが既に ``AcmeFooBundle`` という Bundle を設定しており、 Javascript
    ファイルが ``Resources/public/js`` に保存されていると仮定しています。
    しかし、これは特別重要な事ではありません。 Javascirpt ファイルはどこからでも参照することが出来るからです。

asset タグに追記された ``yui_js`` というフィルタをつけることで、サイズを削減した Javascript が転送されるようになり、
これで Asset ファイルの転送時間が短くなります。これは stylesheet に関しても同様です。

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

サイズ圧縮された Javascript や Stylesheet は非常に読みづらく、言うまでもなくデバッグ時には無効にしたいと思います。
Assetic はデバッグモード時にフィルタを無効にできる機能も備えています。デバッグ時にフィルタを無効
にするにはテンプレートに記述するフィルタ名の前に ``?`` をつけます。これは Assetic にデバッグモードが
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


.. tip::

    asset タグにフィルターを追加する代わりに、フィルターコンフィギュレーション
    に apply-to 属性を追加して、グローバルに有効化することができます。例えば、
     yui_js フィルターに ``apply_to: "\.js$"`` のようにするなどです。
    本番のみにフィルターを適用するには、共通のコンフィグファイルではなく、
    config_prod ファイルに追加します。ファイルの拡張子によるフィルターの適用の
    詳細は、 :ref:`cookbook-assetic-apply-to` を参照してください。


.. _`YUI Compressor`: http://developer.yahoo.com/yui/compressor/
.. _`Download the JAR`: http://yuilibrary.com/downloads/#yuicompressor


.. 2012/10/14 ganchiku c0e8a9a1e77b78d30c4645e144661cc8fafe6ad1
