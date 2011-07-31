Assetの管理にどうやってAsseticを使うか?
=======================================

AsseticはSymfony2 Standard EditionにバンドルされているパワフルなAsset管理をするための
ライブラリでTwigやPHPテンプレートからとても簡単に使うことが出来ます。

Asseticは主にAssetとフィルターの二つの機能をあわせたものです。
AssetとはCSSやJavascript,それに画像などのリソースファイルの事です。
フィルターとはこれらのファイルをブラウザに転送する前に何らかの処理を行う機能です。
AsseticはAssetの保存とユーザーへの表示の処理を分離できることを意味します。

もしAsseticがなかったら、AssetファイルをWeb公開ディレクトリに保存して、
アプリケーションから該当ファイルへのパスを出力するだけになります。

.. configuration-block::

    .. code-block:: html+jinja

        <script src="{{ asset('js/script.js') }}" type="text/javascript" />

    .. code-block:: php

        <script src="<?php echo $view['assets']->getUrl('js/script.js') ?>"
                type="text/javascript" />

Asseticを使う場合はAssetファイルの出力をする前にフィルタ等の処理を追加することが出来ます。

* CSSやJSファイルのサイズを小さくしたり、ひとつのファイルにまとめたりする

* CoffeeScriptなどを使ってJSファイルやCSSを処理できる

* 画像の最適化処理を行える

Assets
------

Asseticは直接Assetファイルを出力するよりも多くの便利な機能を提供します。
例えば、ファイルを外部から直接見れる場所に置く必要がなかったり、Bundleの中に
あるAssetを指定したりすることができます。

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*'
        %}
        <script type="text/javascript" src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*')) as $url): ?>
        <script type="text/javascript" src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

この例では全てのファイルは ``AcmeFooBundle`` の ``Resources/public/js/`` から読み込まれており、
実際にサーバーがユーザーに転送するファイルの場所とは違う場所にあります。Asseticから実際に出力される
タグはこのようにシンプルなものです。

    <script src="/js/abcd123.js"></script>

もちろん複数のファイルをひとつにまとめることも出来ます。 
これは複数のHTTPリクエストを減らしフロントエンドのパフォーマンスの向上につながります。
多くのブラウザでは一度のリクエストで限定した数のリクエストしか行えないようになっているので、
複数のリソースを読み込む場合ページのロードが遅くなってしまいます。
ひとつにまとめられる、ということは管理しやすいパーツに分ける事もできる、という事です。
このように、Assetファイルの再利用性が高いのであるプロジェクトから別のプロジェクトに分けることも
容易に行えます。

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*'
                       '@AcmeBarBundle/Resources/public/js/form.js'
                       '@AcmeBarBundle/Resources/public/js/calendar.js'
        %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*',
                  '@AcmeBarBundle/Resources/public/js/form.js',
                  '@AcmeBarBundle/Resources/public/js/calendar.js')) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

(訳注: AcmeFooBundle/Resources/public/js/thridpartyに他のライブラリなどが入っていると仮定します)
上記の例ではJQuery等のサードパーティのアセットをひとまとめにしません。
もしサードパーティのライブラリも一つのファイルにまとめたいならこのように記述します。

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/thirdparty/jquery.js'
                       '@AcmeFooBundle/Resources/public/js/*'
        %}
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

更にAsseticはAssetファイルが出力される前にフィルターを適用することが出来ます。
これは出力前にファイルサイズの縮小などの最適化が行える、ということです。
また、別のフィルタではJavaScriptを生成するためにCoffeeScriptをつかったり、
SASSをつかってCSSを生成したりすることも可能です。

多くのフィルタは内部に機能を実装しているわけではなく他のライブラリを
使ってこれらの処理を行うため、フィルタを使う場合にはそれらのライブラリの
インストールをする必要があります。
大きな優位点としてはAsseticがこれらのライブラリを使って実行してくれるので、
手動で何かしらの最適化処理などを行う必要がないという事です。
Asseticはこのような面倒な作業も開発中や、デプロイのプロセスとして完全に処理をしてくれます。

フィルタを使う場合はAsseticの設定にフィルタの設定を追加する必要があります。
デフォルトの設定ではフィルタを使うことが出来ません。
例えば、JavaScriptをYUI Compressorを使って処理するには下記のような設定の追加が必要になります。

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

これでテンプレート中でフィルタを適用することが出来ます。

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

YUI Copmressorを使ったもっと詳細なAsseticの説明については :doc:`/cookbook/assetic/yuicompressor`
に記載されています。

AssetのURLを指定する
--------------------

もしAssetが出力するURLを自分で管理したいのであれば、このようにoutputパラメーターを指定し、
公開されるDocument Rootからのパスをテンプレートファイルに書くことで可能です。

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*'
           output='js/combined.js'
        %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array(),
            array('output' => 'js/combined.js')
        ) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

出力のキャッシュ
------------------

ファイルの生成やフィルタの処理などは特に処理時間がかかってしまいます。
開発環境でこのような遅い処理はとくにフラストレーションを溜めてしまい易い部分です。
しかし、安心してください。Asseticはフィルタなどの処理もきちんとキャッシュを
行ってくれるのでフラストレーションを溜めないで済みます。むしろキャッシュを
手動でクリアしたいという場合でもAsseticがファイルの更新の有無を正しくチェックして
処理してくれるのでそもそもキャッシュのクリアが必要ありません。
きちんとしたキャッシュチェック機構があるので、毎回フィルタなどの処理で待たなくとも良くなり、
assetファイルの編集と実際に出力されたページの結果にだけ注力すればよくなります。

プロダクション環境では基本的にAssetファイルの変更は行いません。
ファイルの更新チェックを省くことでパフォーマンスの向上が期待されます。
Asseticは更新チェックを省く設定をするのにSymfony2やPHPファイルを触る必要すらなく、
コンソールからコマンドを実行するだけで全て解決します。
このように、Assetファイルを静的なファイルとして出力することでパフォーマンスの向上を行うことができます。
コンソールコマンドで全てのAssetファイルを処理するには下記のようにコマンドラインに指定します。

.. code-block:: bash

    php app/console assetic:dump

.. note::

    Assetファイルの新しい変更などを知りたい場合は再度コンソールからdumpコマンドを指定
    します。もし開発環境でAssetファイルをコンソールから生成した場合にオンザフライ
    のフィルタ機能を有効にするには生成したAssetファイルを削除する必要があります。
