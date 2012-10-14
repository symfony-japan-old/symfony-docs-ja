FrameworkBundle 設定 ("framework")
========================================

このリファレンス文書はまだ未完成です。本来正確でなければならいないのですが、
すべてのオプションはまだ完全には対応していません。

``FrameworkBundle`` はベースとなるフレームワーク機能のほとんどが含まれおり
アプリケーション設定内の ``フレームワーク`` キーを構成することができます。
これを含む設定の関連に、sessions、transaction、forms、validation、routingなどがあります。

設定
-------------

* `secret`_
* `ide`_
* `test`_
* `trust_proxy_headers`_
* `form`_
    * enabled
* `csrf_protection`_
    * enabled
    * field_name
* `session`_
    * `lifetime`_
* `templating`_
    * `assets_base_urls`_
    * `assets_version`_
    * `assets_version_format`_

secret
~~~~~~

**type**: ``string`` **required**

これはアプリケーションで固有にしなければならない文字列です。

実際には、CSRFトークンを生成するために使われていますが、ユニークな文字列を持つことが
有用な別のコンテキストで使用することができます。それは ``kernel.secret`` という名前の
サービスコンテナにすることができます。

ide
~~~

**type**: ``string`` **default**: ``null``

もしTextMate か Mac VimのようなIDEを使用しているなら、Symfonyは
リンクに例外メッセージ内のファイルパスのすべてを変えることができ、
IDEでそのファイルを開きます。

TextMate か Mac Vimを使用する場合、次のビルトインの値のいずれかを使用することができます。

* ``textmate``
* ``macvim``

さらにカスタイムファイルのリンク文字列を指定することができます。これを行う場合は
エスケープのためすべてのパーセント記号(``%``)を二重にする必要があります。例えば、
完全なTextMateの文字列は次のようになります。

.. code-block:: yaml

    framework:
        ide:  "txmt://open?url=file://%%f&line=%%l"

もちろん、すべての開発者が別々のIDEを使っているので、システムレベルごとに
設定するとよいでしょう。これはファイルリンクの文字列に ``xdebug.file_link_format`` の値
を設定することによって行うことができます。この設定値が設定されている場合、
``ide`` オプションを指定する必要はありません。

.. _reference-framework-test:

.. 2012/10/14 chisei a1aaeb5526d0bf09ec6ef9feb14087fa633b1371 (ideまで翻訳)
