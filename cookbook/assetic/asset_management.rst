.. index::
   single: Assetic; Introduction

Asset の管理にどうやって Assetic を使うか?
=======================================

Assetic は主に Asset とフィルターの二つの機能をあわせたものです。
Asset とは CSS や Javascript 、それに画像などのリソースファイルの事です。
フィルターとはこれらのファイルをブラウザに転送する前に何らかの処理を行う機能です。
Assetic はアプリケーション内での Asset の格納と、実際にユーザーへの表示の処理を分離を可能にします。

もし Assetic がなかったら、 Asset ファイルを Web 公開ディレクトリに保存して、
アプリケーションから該当ファイルへのパスを出力するだけになります。

.. configuration-block::

    .. code-block:: html+jinja

        <script src="{{ asset('js/script.js') }}" type="text/javascript" />

    .. code-block:: php

        <script src="<?php echo $view['assets']->getUrl('js/script.js') ?>" type="text/javascript" />

Assetic を使う場合は Asset ファイルの出力をする前にフィルタ等の処理を追加することが出来ます。

* CSS や JS ファイルのサイズを小さくしたり、ひとつのファイルにまとめたりする

* CoffeeScript などを使ってJSファイルや CSS を処理できる

* 画像の最適化処理を行える

Assets
------

Assetic は直接 Asset ファイルを出力するよりも多くの便利な機能を提供します。
例えば、ファイルを外部から直接見れる場所に置く必要がなかったり、 Bundle の中に
ある Asset ファイルを指定したりすることができます。

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' %}
            <script type="text/javascript" src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*')) as $url): ?>
            <script type="text/javascript" src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. tip::

    `javascripts` タグではなく、 `stylesheets` タグに切り替えれば、
    同様の方法で CSS スタイルシートに Assetic を使用することができます:

    .. configuration-block::

        .. code-block:: html+jinja

            {% stylesheets '@AcmeFooBundle/Resources/public/css/*' %}
                <link rel="stylesheet" href="{{ asset_url }}" />
            {% endstylesheets %}

        .. code-block:: html+php

            <?php foreach ($view['assetic']->stylesheets(
                                                 array('@AcmeFooBundle/Resources/public/css/*')
                                             ) as $url): ?>
                <link rel="stylesheet" href="<?php echo $view->escape($url) ?>" />
            <?php endforeach; ?>

この例では全てのファイルは ``AcmeFooBundle`` の ``Resources/public/js/`` から読み込まれており、
実際にサーバーがユーザーに転送するファイルの場所とは違う場所にあります。Asseticから実際に出力される
タグはこのようにシンプルなものです。:

.. code-block:: html

    <script src="/js/abcd123.js"></script>

.. note::

    キーポイント: Assetic のハンドラを使用してアセットを管理することになると、
    異なる場所からこのアセットファイルを転送することになります。このことによって
    CSS で相対パスを指定して画像を参照している際に *問題となることがあります* 。
    そういった際には、 ``cssrewrite`` フィルターを使用して CSS ファイル内のパス
    を新しい格納先に反映させるようにして、修正することができます。

Assets を結合する
~~~~~~~~~~~~~~~~~

もちろん複数のファイルをひとつにまとめることも出来ます。 
これは複数のHTTPリクエストを減らしフロントエンドのパフォーマンスの向上につながります。
多くのブラウザでは一度のリクエストで限定した数のリクエストしか行えないようになっているので、
複数のリソースを読み込む場合ページのロードが遅くなってしまいます。
ひとつにまとめられる、ということは管理しやすいパーツに分ける事もできる、という事です。
このように、Assetファイルの再利用性が高いのであるプロジェクトから別のプロジェクトに分けることも
容易に行えます。:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts
            '@AcmeFooBundle/Resources/public/js/*'
            '@AcmeBarBundle/Resources/public/js/form.js'
            '@AcmeBarBundle/Resources/public/js/calendar.js' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*',
                  '@AcmeBarBundle/Resources/public/js/form.js',
                  '@AcmeBarBundle/Resources/public/js/calendar.js')) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

`dev` 環境では、それぞれのファイルは別々に転送されますので、デバッグフレンドリーです。
しかし、 `prod` 環境では、これらのファイル群はまとめられ、１つの `script` タグで表示されます。

.. tip::

    Assetic の初心者であれば、実際に ``prod`` 環境で使用してみてください( ``app.php`` 
    コントローラーを指定するなどして)。そうすると CSS や JS が壊れてしまうようにみえることがあります。
    でも、心配しないでください。これはわざとです。 ``prod`` 環境の Assetic の使用の詳細については、
    :ref:`cookbook-assetic-dumping` を参照してください。

そして、ファイルの結合は、 *あなたの* ファイル群のみに適用されるわけではありません。
jQuery などのサードパーティ製の Assets を結合することもこともでき、１つのファイルとして転送することができます。


.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts
            '@AcmeFooBundle/Resources/public/js/thirdparty/jquery.js'
            '@AcmeFooBundle/Resources/public/js/*' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/thirdparty/jquery.js',
                  '@AcmeFooBundle/Resources/public/js/*')) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

フィルター
----------

更に Assetic は Asset ファイルが出力される前にフィルターを適用することが出来ます。
これは出力前にファイルサイズの縮小などの最適化が行える、ということです。
また、別のフィルターでは JavaScript を生成するために CoffeeScript をつかったり、
SASS をつかって CSS を生成したりすることも可能です。
実際には、 Assetic は使用可能なフィルターの長いリストなのです。

多くのフィルターは内部に機能を実装しているわけではなく他のライブラリを
使ってこれらの処理を行うため、フィルターを使う場合にはそれらのライブラリの
インストールをする必要があります。
大きな優位点としては Assetic がこれらのライブラリを使って実行してくれるので、
手動で何かしらの最適化処理などを行う必要がないという事です。
Assetic はこのような面倒な作業も開発中や、デプロイのプロセスとして完全に処理をしてくれます。

フィルターを使う場合は Assetic の設定にフィルターの設定を追加する必要があります。
デフォルトの設定ではフィルターを使うことが出来ません。

例えば、 JavaScript を YUI Compressor を使って処理するには下記のような設定の追加が必要になります。:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                yui_js:
                    jar: "%kernel.root_dir%/Resources/java/yuicompressor.jar"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="yui_js"
                jar="%kernel.root_dir%/Resources/java/yuicompressor.jar" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'yui_js' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
            ),
        ));

これでテンプレート中でフィルターを適用することが出来るようになりました。:

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

YUI Copmressor を使ったもっと詳細な Assetic の説明については :doc:`/cookbook/assetic/yuicompressor`
に記載されています。

AssetのURLを指定する
--------------------

もし Asset が出力するURLを自分で管理したいのであれば、次ように output パラメーターを指定し、
公開される Document Root からのパスをテンプレートファイルに書くことで可能です。

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' output='js/compiled/main.js' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array(),
            array('output' => 'js/compiled/main.js')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. note::

    Symfony には、 *キャッシュバスティング* をする メソッドもあります。
    Assetic によって生成された最終的な URL にクエリーパラメターを付けて
    デプロイ毎のコンフィギュレーションで管理することができます。
    より詳細は、 :ref:`ref-framework-assets-version` の設定オプションを参照してください。

.. _cookbook-assetic-dumping:

アセットファイルのダンプ
------------------------

``dev`` 環境においては Assetic は、実際に存在していない CSS や JavaScript のファイル
へのパスを生成します。その代わりに、 Symfony の内部のコントローラーがファイルを開き
その内容を転送します(全てのフィルターの実行後に)。

こういった動的なアセットファイルの生成による転送は、内容の変更時などを
すぐに見ることができるので素晴らしい機能です。しかし、この処理が遅くなりがちなため
悪い影響も与えてしまいます。特に多くのフィルターを使用しているときにはフラストレーションに
なってしまいます。

幸運にも、 Assetic は、動的に生成する代わりに、アセットファイルを実際のファイルに
ダンプする方法も提供しています。

``prod`` 環境でのアセットファイルのダンプ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``prod`` 環境では、 JS ファイル群と CSS ファイル群は、それぞれ１つのタグで転送します。
つまり、ソースコードに書かれた全ての JavaScript ファイルを見ることなく、次のような１行を見ることになります。:

.. code-block:: html

    <script src="/app_dev.php/js/abcd123.js"></script>

実際には、 このファイルは **存在しません** し、 ``dev`` 環境の アセットファイルのように
Symfony によって動的に表示されません。これはわざとそうしています。本番環境で Symfony に動的にファイル
ファイルを生成させるのは遅くて使い物になりません。

その代わり、 ``prod`` 環境のアプリケーションを使用する度（デプロイする度）に、
次のタスクを実行する必要があります。:

.. code-block:: bash

    $ php app/console assetic:dump --env=prod --no-debug

.. note::

このタスクは ``js/abcd123.js`` などの実際に必要なファイルを、書き出します。
アセットに何か変更があった際には、このタスクを再び実行して、再生成する必要があります。


``dev`` 環境のアセットファイルのダンプ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

デフォルトでは、 ``dev`` 環境内のそれぞれのアセットのパスは、 Symfony によって動的に
出力されます。変更があった際にすぐ反映されるため、不利な点はありませんが、遅くなることは確かです。
この遅さが気になるようでしたら、次のガイドに沿ってみてください。

まず、 Symfony にこれらのファイルを動的に処理することを止めさせます。 ``config_dev.yml`` ファイル
を次のように修正してください。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        assetic:
            use_controller: false

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <assetic:config use-controller="false" />

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('assetic', array(
            'use_controller' => false,
        ));

これで Symfony が動的にファイルを生成することをしなくなりましたので
手動でこれらのファイルをダンプする必要があります。そのためには、次のタスクを
実行してください。:

.. code-block:: bash

    $ php app/console assetic:dump

このタスクは、 ``dev`` 環境用に必要なアセットファイルを全て書き出します。
不利な点として、アセットに変更がある事に、毎回このタスクを実行する必要があることです。
しかし、タスクに ``--watch`` オプションを渡してあげると、 *アセットの変更* がある毎に
自動的に再生成してくれます。:

.. code-block:: bash

    $ php app/console assetic:dump --watch

``dev`` 環境でこのコマンドを実行すると、ファイルをたくさん生成することになるので、
整理するためにも ``/js/compiled`` ディレクトリなどを掘って、そこに生成するように
指定するのが良いでしょう。

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' output='js/compiled/main.js' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array(),
            array('output' => 'js/compiled/main.js')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. 2012/10/14 ganchiku 7c541f2f7542552988cac0a208aafb4841774a41

