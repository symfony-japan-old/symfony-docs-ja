.. note::

    * 対象バージョン：2.3以降
    * 翻訳更新日：2014/11/03

Webアセット
===========

Webアセットとは、CSSやJavaScript、画像といった、Webサイトのフロントエンドの
見た目や操作のためのファイルのことを指します。Symfony開発者は慣習的に、それぞれの
バンドルの ``Resources/public/`` ディレクトリにアセットを配置してきました。

.. best-practice::

    アセットは ``web/`` ディレクトリの中に配置しましょう。

Webアセットを複数の異なるバンドルに跨って分散させてしまうと管理するのが
大変になります。全てのアプリケーションアセットが一箇所にまとまっていれば、
デザイナーの仕事はずっと楽になるでしょう。

アセットを集中管理することで、テンプレートも恩恵を受けることができます。
なぜなら、リンクがより簡潔になるからです。:

.. code-block:: html+jinja

    <link rel="stylesheet" href="{{ asset('css/bootstrap.min.css') }}" />
    <link rel="stylesheet" href="{{ asset('css/main.css') }}" />

    {# ... #}

    <script src="{{ asset('js/jquery.min.js') }}"></script>
    <script src="{{ asset('js/bootstrap.min.js') }}"></script>

.. note::

    ``web/`` は公開ディレクトリであり、配置されているものは全て一般にアクセスできる
    ことに気をつけましょう。そのようあな理由から、コンパイルされたwebアセットだけを
    を配置し、Sassのようなソースファイルは配置するべきではありません。

アセティックを利用する
----------------------

近頃は、単に静的なCSSやJavascriptファイルを作ってテンプレートに埋め込むことは
おそらくしないでしょう。それどころか、クライアントサイドのパフォーマンスを改善
するために、ファイルを結合し圧縮したいと考えることでしょう。
また、LESSやSassのようなものを使いたいと思うかも知れません。それらをCSSファイル
に変換するためには何らかの方法が必要です。

GruntJSのような(PHPではない)ピュアフロントエンドツールを含めて、これらの問題を
解決するためたくさんのツールが存在しています。

.. best-practice::

    GruntJSのようなフロントエンドツールに満足していないなら、Asseticを使って
    webアセットを結合し圧縮しましょう。

`Assetic`_ は、LESSやSassやCoffeScriptといった多くのフロントエンドツールで
開発されたアセットをコンパイルすることができるAssetマネージャです。
Asseticで全てのアセットを結合するには、１つのTwigタグで全てのアセットを
囲みます。

.. code-block:: html+jinja

    {% stylesheets
        'css/bootstrap.min.css'
        'css/main.css'
        filter='cssrewrite' output='css/compiled/all.css' %}
        <link rel="stylesheet" href="{{ asset_url }}" />
    {% endstylesheets %}

    {# ... #}

    {% javascripts
        'js/jquery.min.js'
        'js/bootstrap.min.js'
        output='js/compiled/all.js' %}
        <script src="{{ asset_url }}"></script>
    {% endjavascripts %}

フロントエンドベースのアプリケーション
--------------------------------------

近年、APIと通信を行うフロントエンドWEBアプリケーションを開発する場合、AngularJS
のようなフロントエンド技術を利用することはずいぶん一般的になりました。

もしこのようなアプリケーションを開発する場合には、BowerやGruntJSのような、
その技術で推奨されているツールを使うとよいでしょう。
Symfonyバックエンドとは切り離してフロントエンドアプリケーションを開発するべきです。
バージョン管理システムのリポジトリを分離したい場合にはなおさらです。


Asseticをもっと知るために
-------------------------

Assetic は `UglifyCSS/UglifyJSを使う`_ ことでCSSやJavaScriptのサイズを小さくして
ウェブサイトの高速化を図ることができます。Asseticの `画像の圧縮`_ 機能を使うことで、
ユーザーからリクエストされた画像をその場で圧縮してから返すようにもできます。
利用可能な機能を知りたければ `公式のAsseticドキュメント`_ を参照してください。

.. _`Assetic`: http://symfony.com/doc/current/cookbook/assetic/asset_management.html
.. _`UglifyCSS/UglifyJSを使う`: http://symfony.com/doc/current/cookbook/assetic/uglifyjs.html
.. _`画像の圧縮`: http://symfony.com/doc/current/cookbook/assetic/jpeg_optimize.html
.. _`公式のAsseticドキュメント`: https://github.com/kriswallsmith/assetic

.. 2014/11/03 okapon 2c2000a0274b182cbf1a429badb567ee65432c54
