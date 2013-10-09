.. 2013/10/09 monmonmon 899d0f0d9964aeda17b0716bd772eb75cb304da5

.. note::

    * 対象バージョン：2.3 (2.2以降)
    * 翻訳更新日：2013/10/09

.. index::
   single: Assetic; UglifyJs


UglifyJs と UglifyCss を使って Javascript や Stylesheet のサイズを圧縮するには?
===============================================================================

`UglifyJs`_ は JavaScript の構文解析、圧縮、整形などを行うツールです。
JavaScript ファイルを結合、圧縮し、HTTP リクエスト数を減らしてロード時間を抑えることが出来ます。
`UglifyCss`_ は CSS の圧縮、整形などを行うツールで、\ ``UglifyJs`` によく似ています。

このクックブックでは ``UglifyJs`` のインストール、設定、使い方について詳細に記述します。
``UglifyCss`` は ``UglifyJs`` と同様に利用できるため、ごく簡単に説明します。

UglifyJs のインストール
-----------------------

UglifyJs は `Node.js`_ の npm モジュールとして提供されており、 npm コマンドを使ってインストール出来ます。
まずは `Node.js`_, `npm`_ をインストールして下さい。
npm コマンドで UglifyJs を次のようにインストール出来ます。

.. code-block:: bash

    $ npm install -g uglify-js

これで UglifyJs がグローバルにインストールされます（root 権限が必要かもしれません）。

.. note::

    UglifyJs をグローバルにインストールする代わりに、プロジェクトでのみ使えるようにプロジェクトのローカルにインストールすることも出来ます。
    ``-g`` オプションなしで、インストール先を指定して npm を実行して下さい。

    .. code-block:: bash

        $ cd /path/to/symfony
        $ mkdir app/Resources/node_modules
        $ npm install uglify-js --prefix app/Resources

    バージョン管理のため、\ ``app/Resources`` の下に ``node_modules`` ディレクトリを作って、そこに UglifyJs をインストールすることが推奨されます。
    もしくは npm の `package.json`_ ファイルを作って、ここにモジュール間の依存関係を記述することも出来ます。

グローバルにインストールするかローカルにインストールするかによって ``uglifyjs`` 実行ファイルのパスは異なります。

.. code-block:: bash

    $ uglifyjs --help                                       # グローバルにインストールした場合

    $ ./app/Resources/node_modules/.bin/uglifyjs --help     # ローカルにインストールした場合

uglifyjs2 フィルタの設定
------------------------

では Symfony2 で ``uglifyjs2`` フィルタを使えるようにコンフィギュレーションファイルに設定しましょう。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                uglifyjs2:
                    # uglifyjs 実行ファイルのパス
                    bin: /usr/local/bin/uglifyjs

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="uglifyjs2"
                bin="/usr/local/bin/uglifyjs" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'uglifyjs2' => array(
                    'bin' => '/usr/local/bin/uglifyjs',
                ),
            ),
        ));

.. note::

    UglifyJs をグローバルにインストールした場合、uglifyjs 実行ファイルがどこに設置されるかはシステム依存です。
    npm が管理する ``bin`` ディレクトリを以下のコマンドで調べて下さい。uglifyjs 実行ファイルはこのディレクトリの下にインストールされます。

    .. code-block:: bash

        $ npm bin -g

    UglifyJs をローカルにインストールした場合、\ ``bin`` ディレクトリは ``app/Resources/node_modules/.bin`` です。

これで ``uglifyjs2`` フィルタを利用出来るようになりました。

実際に JavaScript ファイルを圧縮
--------------------------------

テンプレート中の ``javascripts`` タグに以下のように書くことで ``uglifyjs2`` フィルタを適用出来ます。

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' filter='uglifyjs2' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array('uglifyj2s')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. note::

    上記の例では ``AcmeFooBundle`` バンドルの下の ``Resources/public/js`` ディレクトリに JavaScript ファイルがあることを前提としていますが、
    JavaScript ファイルはどこにあっても構いません。

これで、\ ``uglifyjs2`` フィルタにより圧縮された JavaScript がダウンロードされるようになることが確認出来ると思います。

デバッグモードで圧縮を無効化
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

圧縮された JavaScript は非常に読みづらく、デバッグも困難です。
これを回避するため、Assetic ではデバッグモード (例: ``app_dev.php``) の時に特定のフィルタを無効に出来ます。
テンプレート中に記述したフィルタ名の頭に ``?`` を付けることで、デバッグモードでない場合 (例: ``app.php``) のみこのフィルタが有効になります。

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' filter='?uglifyjs2' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array('?uglifyjs2')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

確認するには、\ ``prod`` 環境 (``app.php``) に切り替えてみて下さい。
:ref:`キャッシュをクリアする <book-page-creation-prod-cache-clear>` のと :ref:`Asset ファイルをダンプする <cookbook-asetic-dump-prod>` のを忘れないようにして下さい。

.. tip::

    Asset タグ（ここでは ``javascripts`` タグ）にフィルタ設定を書く代わりに、
    コンフィギュレーションファイルのフィルタの設定に apply_to 属性を追加することでフィルタをまとめて適用することも出来ます。
    例えば ``uglifyjs2`` なら ``apply_to: "\.js$"`` のように書きます。
    prod 環境でのみこれを適用するのであれば、\ ``config_prod`` コンフィギュレーションファイルに書きます。
    ファイル拡張子によるフィルタの適用について、詳細は :ref:`cookbook-assetic-apply-to` を参照して下さい。

UglifyCss のインストール、設定、利用方法
----------------------------------------

UglifyCss は UglifyJs と同様に使えます。
まず npm で node パッケージをインストールして下さい。

.. code-block:: bash

    $ npm install -g uglifycss

次にコンフィギュレーションファイルにフィルタの設定を記述します。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                uglifycss:
                    bin: /usr/local/bin/uglifycss

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="uglifycss"
                bin="/usr/local/bin/uglifycss" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'uglifycss' => array(
                    'bin' => '/usr/local/bin/uglifycss',
                ),
            ),
        ));

圧縮したい css の ``stylesheets`` タグに以下のようにフィルタ設定を追加して下さい。

.. configuration-block::

    .. code-block:: html+jinja

        {% stylesheets '@AcmeFooBundle/Resources/public/css/*' filter='uglifycss' %}
             <link rel="stylesheet" href="{{ asset_url }}" />
        {% endstylesheets %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->stylesheets(
            array('@AcmeFooBundle/Resources/public/css/*'),
            array('uglifycss')
        ) as $url): ?>
            <link rel="stylesheet" href="<?php echo $view->escape($url) ?>" />
        <?php endforeach; ?>

``uglifyjs2`` と同様、フィルタ名の頭に ``?`` を付けることで (``?uglifycss`` とすることで) デバッグモードでない時のみ圧縮が有効になります。

.. _`UglifyJs`: https://github.com/mishoo/UglifyJS
.. _`UglifyCss`: https://github.com/fmarcia/UglifyCSS
.. _`Node.js`: http://nodejs.org/
.. _`npm`: https://npmjs.org/
.. _`package.json`: http://package.json.nodejitsu.com/
