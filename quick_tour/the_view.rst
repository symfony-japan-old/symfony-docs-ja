.. 2011/05/01 hidenorigoto 9ca7ce48

ビュー
======

チュートリアルの 2 つめのパートでは、Symfony2 のテンプレートエンジンである `Twig`_ に焦点をあてて学習します。
Twig は柔軟で高速、かつセキュアな PHP 用のテンプレートエンジンです。
Twig を使うとテンプレートの可読性が向上し、簡潔になります。
ですので、Web デザイナーとって扱いやすくなります。

.. note::

    Twig の代わりに :doc:`PHP </cookbook/templating/PHP>` 形式のテンプレートを使うこともできます。
    どちらのテンプレートエンジンも Symfony2 でネイティブにサポートされています。

Twig に慣れる
-------------

.. tip::

    Twig について学習したい方は、\ `Twig の公式ドキュメント`_ を読むことをおすすめします。
    この章では、Twig の主要な概念のみを簡単に説明します。

Twig のテンプレートは、テキストベースの任意の形式（HTML、XML、CSV、LaTeX 等）を生成するためのテキストファイルです。
Twig では、次の 2 種類の区切り文字が使われます。

* ``{{ ... }}``: 変数や式の結果を出力します。

* ``{% ... %}``: テンプレートのロジックを制御するタグで、たとえば ``for`` ループや ``if`` 文を記述します。

Twig のいくつかの基本的な使い方を知るために、次の小さなテンプレートから始めましょう。

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html>
        <head>
            <title>My Webpage</title>
        </head>
        <body>
            <h1>{{ page_title }}</h1>

            <ul id="navigation">
                {% for item in navigation %}
                    <li><a href="{{ item.href }}">{{ item.caption }}</a></li>
                {% endfor %}
            </ul>
        </body>
    </html>


.. tip::

   ``{# ... #}`` 区切り文字を使うと、テンプレートにコメントを記述できます。

テンプレートをレンダリングするには、コントローラから ``render`` メソッドを呼び出します。テンプレートに必要な変数は、このメソッドの引数で渡します。

::

    $this->render('AcmeDemoBundle:Demo:hello.html.twig', array(
        'name' => $name,
    ));

テンプレートには、文字列変数、配列変数、オブジェクト変数を渡すことができます。
Twig では変数の種類の違いを抽象化しており、それらの "属性" にアクセスするにはドット表記 (``.``) を使います。

.. code-block:: jinja

    {# array('name' => 'Fabien') #}
    {{ name }}

    {# array('user' => array('name' => 'Fabien')) #}
    {{ user.name }}

    {# force array lookup #}
    {{ user['name'] }}

    {# array('user' => new User('Fabien')) #}
    {{ user.name }}
    {{ user.getName }}

    {# force method name lookup #}
    {{ user.name() }}
    {{ user.getName() }}

    {# pass arguments to a method #}
    {{ user.date('Y-m-d') }}

.. note::

    波括弧は変数の一部ではなく、表示用の文であると理解しておくことが重要です。
    波括弧のタグの内部で変数にアクセスしたい場合、そこには波括弧は必要ありません。

テンプレートをデコレートする
----------------------------

プロジェクトでは、ヘッダーやフッターといった共通のエレメントを複数のテンプレートで共有したい場合がよくあります。
Symfony2 では、この問題を少し異なる視点で扱います。Symfony2 のテンプレートは、別のテンプレートでデコレートできます。
この機能は、PHP のクラスと同じような概念で機能します。
テンプレートの継承により、Web サイトの共通エレメントをすべて含み、子のテンプレート側でオーバーライドできる "ブロック" を定義したベースの "レイアウト" テンプレートを作ることができます。

次の ``hello.html.twig`` テンプレートは、\ ``extends`` タグにより ``layout.html.twig`` を継承するよう指定されています。

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {% block title "Hello " ~ name %}

    {% block content %}
        <h1>Hello {{ name }}!</h1>
    {% endblock %}

テンプレートファイルを指定する ``AcmeDemoBundle::layout.html.twig`` という記法は分かりやすいですね。
これは、コントローラでテンプレートファイルを参照する時に使ったのと同じ記法です。
``::`` という部分は、コントローラ名に該当する部分が空であることを意味し、対応するファイルは ``views/`` ディレクトリ直下に配置されます。

それでは、単純化した ``layout.html.twig`` ファイルの中身を見てみましょう。

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/layout.html.twig #}
    <div class="symfony-content">
        {% block content %}
        {% endblock %}
    </div>

``{% block %}`` タグは、ブロックを定義します。
ブロックの内容は、子のテンプレート側で内容を置き換えることができます。
ブロックタグで行われるのは、ブロックの部分が子テンプレートで置き換え可能であることをテンプレートエンジンに知らせることだけです。
``hello.html.twig`` テンプレートは、\ ``content`` ブロックをオーバーライドしています。

Twig のタグ、フィルター、関数を使う
-----------------------------------

Twig の素晴らしい機能の 1 つに、タグ、フィルター、関数を使った拡張性の高さがあります。
テンプレートデザイナーの作業を簡単にするために、Symfony2 には多くのタグ、フィルター、関数が組み込まれています。

別のテンプレートをインクルードする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

複数のテンプレートでコード片を共有する簡単な方法は、新しいテンプレートを作成し、別のテンプレートからそれをインクルードすることです。

``embedded.html.twig`` テンプレートを次の内容で作成します。

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/embedded.html.twig #}
    Hello {{ name }}

これをインクルードするように ``index.html.twig`` テンプレートを変更します。

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {# override the body block from embedded.html.twig #}
    {% block content %}
        {% include "AcmeDemoBundle:Demo:embedded.html.twig" %}
    {% endblock %}

別のコントローラの実行結果を埋め込む
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

もし別のコントローラの実行結果をテンプレートに埋め込みたい場合はどうしますか？
これは Ajax を使っている場合や、メインのテンプレートにはない変数を埋め込むテンプレートで使いたい場合にとても便利です。

``fancy`` アクションを作り、この結果を ``index`` テンプレートの中に埋め込みたいとします。
この場合、\ ``render`` タグを使って次のように記述します。

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/index.html.twig #}
    {% render "AcmeDemoBundle:Demo:fancy" with { 'name': name, 'color': 'green' } %}

ここで使われている ``AcmeDemoBundle:Demo:fancy`` という文字列は、\ ``Demo`` コントローラの ``fancy`` アクションを参照しています。
引数（\ ``name`` と ``color``\ ）はリクエスト変数のように（\ ``fancyAction`` が新しいリクエストを処理しているように）処理され、コントローラで利用できます。

::

    // src/Acme/DemoBundle/Controller/DemoController.php

    class DemoController extends Controller
    {
        public function fancyAction($name, $color)
        {
            // create some object, based on the $color variable
            $object = ...;

            return $this->render('AcmeDemoBundle:Demo:fancy.html.twig', array('name' => $name, 'object' => $object));
        }

        // ...
    }

ページ間のリンクを作成する
~~~~~~~~~~~~~~~~~~~~~~~~~~

Web アプリケーションでは、ページ間のリンク作成は必須の機能です。
テンプレートに URL をハードコーディングするのではなく、\ ``path`` 関数を使ってルーティングコンフィギュレーションに基づいて URL を生成できます。
こうすることで、コンフィギュレーションを変更するだけで簡単にすべての URL を更新できるようになります。

.. code-block:: html+jinja

    <a href="{{ path('_demo_hello', { 'name': 'Thomas' }) }}">Greet Thomas!</a>

``path`` 関数は、ルート名とパラメータの配列を引数にとります。
ルート名引数で、どのルートを使うのかが決定され、パラメータの配列は、ルートパターンに定義されたプレースホルダーの値として使われます。

::

    // src/Acme/DemoBundle/Controller/DemoController.php
    /**
     * @extra:Route("/hello/{name}", name="_demo_hello")
     * @extra:Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

.. tip::

    ``url`` 関数は\ *絶対* URL を生成します: ``{{ url('_demo_hello', {
    'name': 'Thomas' }) }}``

アセット（画像、JavaScript、スタイルシート）をインクルードする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

インターネットには画像、JavaScript、スタイルシートも使われるでしょう。
Symfony2 では、\ ``asset`` 関数を使ってアセットを簡単に扱えます。

.. code-block:: jinja

    <link href="{{ asset('css/blog.css') }}" rel="stylesheet" type="text/css" />

    <img src="{{ asset('images/logo.png') }}" />

``asset`` 関数の主な目的は、アプリケーションのポータビリティを向上させることです。
この関数を使うと、アプリケーションのルートディレクトリを Web ルートディレクトリの別の場所へ移動させる場合でも、テンプレートのコードを変更する必要がなくなります。

変数をエスケープする
--------------------

Twig は、デフォルトですべての出力をエスケープするように設定されています。
Twig のエスケープ機能と Escaper エクステンションに関する詳細は、\ `Twig の公式ドキュメント`_ を参照してください。

まとめ
------

Twig はシンプルかつ強力です。
レイアウトやブロックの機能、テンプレートやアクションのインクルード機能を使うと、テンプレートをより論理的で拡張しやすい方法で管理できます。

ここまで読まれた方は、まだ Symfony2 について 20 分しか学習していないかもしれませんが、とても魅力的な機能を試しました。
Symfony2 の基礎的な内容は、とても簡単に学ぶことができます。
また、このようなシンプルさが、Symfony2 の柔軟なアーキテクチャによって支えられていることもすぐ後の章で学びます。

しかし、慌てすぎてはいけません。
このチュートリアルの次のトピックとして、次の章でコントローラについてもう少し学習しましょう。
もう 10 分、Symfony2 を学ぶ準備ができたら、次の章へ進んでください。

.. _Twig:          http://www.twig-project.org/
.. _Twig の公式ドキュメント: http://www.twig-project.org/documentation
