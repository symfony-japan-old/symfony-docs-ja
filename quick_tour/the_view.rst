.. note::

    * 対象バージョン：2.3以降
    * 翻訳更新日：2013/11/23

ビュー
======

チュートリアルの 2 つめのパートでは、Symfony2 のテンプレートエンジンである `Twig`_ に焦点をあてて学習します。
Twig は柔軟で高速、かつセキュアな PHP 用のテンプレートエンジンです。
Twig を使うとテンプレートの可読性が向上し、簡潔になります。
ですので、Web デザイナーにとっても扱いやすくなります。

.. note::

    Twig の代わりに :doc:`PHP </cookbook/templating/PHP>` 形式のテンプレートを使うこともできます。
    どちらのテンプレートエンジンも Symfony2 でネイティブにサポートされています。

Twig に慣れる
-------------

.. tip::

    Twig について学習したい方は、\ `Twig の公式ドキュメント`_\ を読むことをおすすめします。
    この章では、Twig の主要な概念のみを簡単に説明します。

Twig のテンプレートは、任意の形式（HTML、XML、CSV、LaTeX 等）を生成するためのテキストファイルです。
Twig では、次の 2 種類の区切り文字の組み合わせが使われます。

* ``{{ ... }}``: 変数や式の結果を出力します。

* ``{% ... %}``: テンプレートのロジックを制御します。たとえば ``for`` ループや ``if`` 文を記述します。

Twig のいくつかの基本的な使い方を知るために、次の小さなテンプレートから始めましょう。このテンプレートでは ``page_title`` と ``navigation`` の 2 つの変数を使います（変数の値はコントローラーから渡されます）。

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

Symfony でテンプレートをレンダリングするには、コントローラーから ``render`` メソッドを呼び出します。テンプレートに必要な変数は、次のように ``render`` メソッドの引数で渡します。

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

    {# 配列要素の参照を明示的に指定 #}
    {{ user['name'] }}

    {# array('user' => new User('Fabien')) #}
    {{ user.name }}
    {{ user.getName }}

    {# メソッド名での参照を明示的に指定 #}
    {{ user.name() }}
    {{ user.getName() }}

    {# メソッドへ引数を渡す #}
    {{ user.date('Y-m-d') }}

.. note::

    波括弧は変数の一部ではなく、表示用の文であると理解しておくことが重要です。
    波括弧のタグの内部で変数にアクセスしたい場合、そこには波括弧は必要ありません。

テンプレートをデコレートする
----------------------------

プロジェクトでは、ヘッダーやフッターといった共通の要素を複数のテンプレートで共有したい場合がよくあります。
Symfony2 では、この問題を少し異なる視点で扱います。テンプレートを、別のテンプレートでデコレートすると考えます。
これは、PHP のクラスおよびその継承に似た概念として機能します。
テンプレートの継承を使う場合、Web サイトの共通要素をベースとなる "レイアウト" テンプレートに定義しておきます。子のテンプレートごとに異なる部分は、子のテンプレート側でオーバーライドできるようにレイアウトテンプレートに "ブロック" を定義しておきます。

次の ``hello.html.twig`` テンプレートは、\ ``extends`` タグにより ``layout.html.twig`` を継承するよう指定されています。

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {% block title "Hello " ~ name %}

    {% block content %}
        <h1>Hello {{ name }}!</h1>
    {% endblock %}

継承するテンプレートファイルを指定する ``AcmeDemoBundle::layout.html.twig`` という記法は、コントローラーでテンプレートファイルを参照する時に使ったのと同じく論理名の記法です。
``::`` という部分は、コントローラー名に該当する部分が空であることを意味し、対応するファイルは ``Resources/views/`` ディレクトリ直下に配置されます。

それでは、単純化した ``layout.html.twig`` ファイルの中身を見てみましょう。

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/layout.html.twig #}
    <div class="symfony-content">
        {% block content %}
        {% endblock %}
    </div>

上の例の ``{% block %}`` タグは、ブロックを定義しています。
ブロックの内容は、子のテンプレート側で内容を置き換えることができます。
ブロックタグで行われるのは、ブロックの部分が子テンプレートで置き換え可能であることをテンプレートエンジンに知らせることだけです。

この例では、\ ``hello.html.twig`` テンプレートの ``content`` ブロックの内容で上書きされます。つまり、"Hello Fabien" というテキストは ``div.symfony-content`` 要素内にレンダリングされます。

Twig のタグ、フィルター、関数を使う
-----------------------------------

Twig の素晴らしい機能の 1 つに、タグ、フィルター、関数を使った拡張性の高さがあります。
テンプレートデザイナーの作業を簡単にするために、Symfony2 には多くのタグ、フィルター、関数が組み込まれています。

別のテンプレートをインクルードする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

複数のテンプレートでコード片を共有する簡単な方法は、共通部分を新しいテンプレートとして作成し、それをインクルードすることです。

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
        {{ include("AcmeDemoBundle:Demo:embedded.html.twig") }}
    {% endblock %}

別のコントローラの実行結果を埋め込む
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

もし別のコントローラーの実行結果をテンプレートに埋め込みたい場合はどうしますか？
これは Ajax を使っている場合や、メインのテンプレートにはない変数を埋め込むテンプレートで使いたい場合にとても便利です。

``fancyAction`` という名前のコントローラーメソッドを作成し、\ ``index`` テンプレート内にレンダリングしたいとします。コントローラーの実行結果（たとえば ``HTML``\ ） を埋め込む必要があります。この場合、テンプレートで ``render`` 関数を使います。

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/index.html.twig #}
    {{ render(controller("AcmeDemoBundle:Demo:fancy", {'name': name, 'color': 'green'})) }}

ここで使われている ``AcmeDemoBundle:Demo:fancy`` という文字列は、\ ``Demo`` コントローラーの ``fancy`` アクションを参照しています。
render 関数に渡している引数 ``name`` と ``color`` は、\ ``fancyAction`` が新しい HTTP リクエストを処理しているかのように処理され、コントローラーから利用できます。

::

    // src/Acme/DemoBundle/Controller/DemoController.php

    class DemoController extends Controller
    {
        public function fancyAction($name, $color)
        {
            // create some object, based on the $color variable
            $object = ...;

            return $this->render('AcmeDemoBundle:Demo:fancy.html.twig', array(
                'name' => $name,
                'object' => $object,
            ));
        }

        // ...
    }

ページ間のリンクを作成する
~~~~~~~~~~~~~~~~~~~~~~~~~~

Web アプリケーションでは、ページ間のリンク作成は必須の機能です。
Symfony ではテンプレートで ``path`` 関数を使うことで、テンプレートに URL をハードコーディングするのではなく、ルーティングコンフィギュレーションに基づいて URL を生成できます。
こうすることで、コンフィギュレーションを変更するだけですべての URL を簡単に更新できるようになります。

.. code-block:: html+jinja

    <a href="{{ path('_demo_hello', { 'name': 'Thomas' }) }}">Greet Thomas!</a>

``path`` 関数は、ルート名とパラメータの配列を引数にとります。
ルート名引数で、どのルートを使うのかが決定され、パラメータの配列は、ルートパターンに定義されたプレースホルダーの値として使われます。

::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    // ...

    /**
     * @Route("/hello/{name}", name="_demo_hello")
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

.. tip::

    ``url`` 関数は\ *絶対* URL を生成します: ``{{ url('_demo_hello', {'name': 'Thomas'}) }}``

アセット（画像、JavaScript、スタイルシート）をインクルードする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Web サイトには画像、JavaScript、スタイルシートも使われるでしょう。
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
しかし、Twig を使いづらいと感じるのであれば、Symfony では PHP テンプレートに切り替えることはいつでもできます。

ここまで読まれた方は、まだ Symfony2 について 20 分しか学習していないかもしれませんが、とても魅力的な機能を試しました。
Symfony2 の基礎的な内容は、とても簡単に学ぶことができます。
また、このようなシンプルさが、Symfony2 の柔軟なアーキテクチャによって支えられていることもすぐ後の章で学びます。

しかし、慌てすぎてはいけません。
次のトピックとして、\ :doc:`チュートリアルの次の章 <the_controller>`\ でコントローラについてもう少し学習しましょう。

.. _Twig:                    http://twig.sensiolabs.org/
.. _Twig の公式ドキュメント: http://twig.sensiolabs.org/documentation

.. 2013/11/23 hidenorigoto 899d0f0d9964aeda17b0716bd772eb75cb304da5
