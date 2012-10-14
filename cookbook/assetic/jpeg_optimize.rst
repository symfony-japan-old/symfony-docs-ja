.. index::
   single: Assetic; Image optimization

Twig の関数で画像の最適化に Assetic をどうやって使うか?
====================================================

沢山のフィルタの中で Assetic はオンザフライでの画像の最適化に使えるフィルタを4つ持っています。
このフィルタを使うことでフォトレタッチソフトを使わなくても簡単に画像のサイズを最適化できます。
最適化された結果はキャッシュ、もしくは指定したディレクトリに保存することが出来るので
プロダクション環境でのパフォーマンスに影響しません。

Jpegoptim を使う
---------------

`Jpegoptim`_ は JPEG ファイルの最適化の為のツールです。 Assetic でこのフィルタを使うには
下記のような Assetic の設定を追加します。:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                jpegoptim:
                    bin: path/to/jpegoptim

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="jpegoptim"
                bin="path/to/jpegoptim" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'jpegoptim' => array(
                    'bin' => 'path/to/jpegoptim',
                ),
            ),
        ));

.. note::

    jpegoptim を使うには既にあなたの環境に jpegoptim がインストールされている
    必要があります。 設定の ``bin`` にあなたの環境で jpegoptim がインストールされている
    場所を指定してください。

これでテンプレートから jpegoptim を使うことが出来ます。

.. configuration-block::

    .. code-block:: html+jinja

        {% image '@AcmeFooBundle/Resources/public/images/example.jpg'
            filter='jpegoptim' output='/images/example.jpg' %}
            <img src="{{ asset_url }}" alt="Example"/>
        {% endimage %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->images(
            array('@AcmeFooBundle/Resources/public/images/example.jpg'),
            array('jpegoptim')) as $url): ?>
            <img src="<?php echo $view->escape($url) ?>" alt="Example"/>
        <?php endforeach; ?>

全ての EXIF データを削除する
~~~~~~~~~~~~~~~~~~~~~~~~~~

初期状態ではこのフィルタはいくつかのメタ情報だけを削除するようにしています。
それ以外の EXIF データやコメントは削除されません。 ``strip_all`` option を
つけることで全ての EXIF データを削除することが出来ます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                jpegoptim:
                    bin: path/to/jpegoptim
                    strip_all: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="jpegoptim"
                bin="path/to/jpegoptim"
                strip_all="true" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'jpegoptim' => array(
                    'bin' => 'path/to/jpegoptim',
                    'strip_all' => 'true',
                ),
            ),
        ));

最適化クオリティを下げる
~~~~~~~~~~~~~~~~~~~~~~~~

初期設定では JPEG の最適化レベルは変更されません。現在の画像の最適化レベルを
下げる事でファイルサイズを更に小さくすることが出来ます。しかしこれは
画像の見た目とのトレードオフとなってしまいます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                jpegoptim:
                    bin: path/to/jpegoptim
                    max: 70

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="jpegoptim"
                bin="path/to/jpegoptim"
                max="70" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'jpegoptim' => array(
                    'bin' => 'path/to/jpegoptim',
                    'max' => '70',
                ),
            ),
        ));

Twig Function のより短い書き方
-----------------------------

もし Twig を使っているのであれば下記の設定をすることでより短いフィルタ名
を使うことが出来ます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                jpegoptim:
                    bin: path/to/jpegoptim
            twig:
                functions:
                    jpegoptim: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="jpegoptim"
                bin="path/to/jpegoptim" />
            <assetic:twig>
                <assetic:twig_function
                    name="jpegoptim" />
            </assetic:twig>
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'jpegoptim' => array(
                    'bin' => 'path/to/jpegoptim',
                ),
            ),
            'twig' => array(
                'functions' => array('jpegoptim'),
                ),
            ),
        ));

この設定を使うことで下記のような書き方でAsseticを使えるようになります。

.. code-block:: html+jinja

    <img src="{{ jpegoptim('@AcmeFooBundle/Resources/public/images/example.jpg') }}" alt="Example"/>

下記の設定をすることでキャッシュの生成ディレクトリを指定することもできます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                jpegoptim:
                    bin: path/to/jpegoptim
            twig:
                functions:
                    jpegoptim: { output: images/*.jpg }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="jpegoptim"
                bin="path/to/jpegoptim" />
            <assetic:twig>
                <assetic:twig_function
                    name="jpegoptim"
                    output="images/*.jpg" />
            </assetic:twig>
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'jpegoptim' => array(
                    'bin' => 'path/to/jpegoptim',
                ),
            ),
            'twig' => array(
                'functions' => array(
                    'jpegoptim' => array(
                        output => 'images/*.jpg'
                    ),
                ),
            ),
        ));

.. _`Jpegoptim`: http://www.kokkonen.net/tjko/projects.html

.. 2012/10/14 ganchiku c0e8a9a1e77b78d30c4645e144661cc8fafe6ad1

