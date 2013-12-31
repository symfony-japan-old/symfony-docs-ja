.. index::
   single: Templating

.. note::

    * 対象バージョン：2.4 (2.1以降)
    * 翻訳更新日：2013/12/31

テンプレートの基本
==================

:doc:`コントローラー </book/controller>` は、\
Symfony2 アプリケーションに入ってきたリクエストを扱う役割を果たします。\
ただし、実際は、すべてをコントローラーに記述するのではなく、コードのテストのしやすさや再利用性のために、重い処理を別の部分に任せていることもあります。\
HTML や CSS その他のコンテンツを生成する処理は、コントローラーからテンプレートエンジンに引き継がれます。\
本章では、ユーザーに提示するコンテンツやメール本文などに使うテンプレートの記述方法を解説します。\
テンプレートを継承したりコードを再利用する方法も説明しています。

.. note::

    テンプレートをレンダリングする方法については、\ :ref:`コントローラー <controller-rendering-templates>`\ の章でも解説しています。

.. index::
   single: Templating; What is a template?

テンプレート
------------

テンプレートとは、任意のテキストベースのフォーマット(HTML、XML、CSV、LaTeX ...)を生成する元となる、シンプルなテキストファイルです。\
一番身近なのは PHP テンプレートでしょう。\
これは PHP によってパースされるテキストファイルで、テキストと PHP コードが混ざっています。

.. code-block:: html+php

    <!DOCTYPE html>
    <html>
        <head>
            <title>Symfony へようこそ!</title>
        </head>
        <body>
            <h1><?php echo $page_title ?></h1>

            <ul id="navigation">
                <?php foreach ($navigation as $item): ?>
                    <li>
                        <a href="<?php echo $item->getHref() ?>">
                            <?php echo $item->getCaption() ?>
                        </a>
                    </li>
                <?php endforeach; ?>
            </ul>
        </body>
    </html>

.. index:: Twig; Introduction

Symfony2 では、より強力なテンプレート言語である `Twig`_ も利用可能です。\
Twig は、簡潔で可読性の高いテンプレートを記述でき、\
Web デザイナーにも使いやすいよう設計されています。\
PHP テンプレートより強力な点もあります。

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html>
        <head>
            <title>Symfony へようこそ!</title>
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

Twig には 2 種類の構文があります。

* ``{{ ... }}``: "出力する": 変数や式の結果をテンプレートに表示する

* ``{% ... %}``: "処理する": テンプレートのロジックを制御する\ **タグ**\で、for ループなどの命令の実行に使用する

.. note::

   もう一つ、次のようなコメント用の構文もあります。

   .. code-block:: html+jinja

       {# これはコメントです #}

   この構文では、PHP の ``/* comment */`` と同じように複数行に渡るコメントも記述できます。

Twig には\ **フィルター**\ という機能もあります。フィルターを使うと、レンダリングされる前にコンテンツを修飾することができます。\
次の例では、フィルターを使って変数 ``title`` の内容をすべて大文字に変換して出力します。

.. code-block:: jinja

    {{ title|upper }}

デフォルトで利用可能な\ `タグ`_\ や\ `フィルター`_\ は数多くあります。\
また、必要であれば、自分で Twig の\ `エクステンションを追加する`_\ ことも可能です。

.. tip::

    作成した Twig エクステンションを登録するには、
    サービスに ``twig.extension`` という\ :ref:`タグ <reference-dic-tags-twig-extension>`\ を設定します。

この後にも出てきますが、Twig は関数をサポートしています。\
また、独自の関数を追加することもできます。\
次の例では、デフォルトのタグ ``for``  と関数 ``cycle`` を使って、\
10 個の div タグを出力しています。\
``cycle`` 関数を使っているので、ループ内で出力される div タグの class 属性として ``odd`` と ``even`` が交互に適用されます。

.. code-block:: html+jinja

    {% for i in 0..10 %}
        <div class="{{ cycle(['odd', 'even'], i) }}">
          <!-- 何らかの HTML コード -->
        </div>
    {% endfor %}

この章では、テンプレートの例は Twig と PHP の両方で示していきます。

.. tip::

    プロジェクトで Twig を使用せず無効にしている場合、\ ``kernel.exception`` イベントを使って例外ハンドラーを自作する必要があります。

.. sidebar:: Twig のメリット

    Twig テンプレートはシンプルに記述することが目的で、テンプレート中で PHP タグを処理することはありません。\
    Twig は見た目の表現手段であって、プログラムロジックの表現手段ではない、という設計思想が根底にあります。
    Twig を使い込んでいくと、この関心事の分離のための設計が効果を実感できるでしょう。

    また、PHP テンプレートにはない機能もあります。例えば、\
    テンプレート継承 (Twig テンプレートは、継承関係のついた PHP クラスにコンパイルされる) 、\
    空白文字の制御、サンドボックス、テンプレート内でのみ有効なカスタム関数やフィルターといった機能があります。\
    他にも、テンプレートの記述を容易で簡潔にする仕組みがいくつかあります。\
    次の例では、論理命令である ``if`` をループに一体化させています。

    .. code-block:: html+jinja

        <ul>
            {% for user in users if user.active %}
                <li>{{ user.username }}</li>
            {% else %}
                <li>ユーザーが見つかりません</li>
            {% endfor %}
        </ul>

.. index::
   pair: Twig; Cache

Twig テンプレートのキャッシュ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Twig は高速です。各テンプレートはネイティブな PHP クラスにコンパイルされ、実行時にレンダリングされます。\
コンパイルされたクラスは、\ ``app/cache/{environment}/twig`` (``{environment}`` は ``dev`` や ``prod`` のような環境を指す) に保存されます。
デバッグ時にこのディレクトリを参照すると役立つ場合もあります。\
環境についての詳細は、\ :ref:`environments-summary` を参照してください。

``debug`` モードが有効になっている場合 (デフォルトの ``dev`` 環境)、\
Twig テンプレートは、変更されていれば自動的に再コンパイルされます。\
ですので、開発中は明示的にキャッシュクリアを行う必要はなく、テンプレートに加えた変更を即座に確認できます。

``debug`` モードが無効の場合 (デフォルトの ``prod`` 環境)、\
Twig テンプレートを再コンパイルするには Twig のキャッシュをクリアする必要があります。\
アプリケーションをデプロイするときは、必ずキャッシュをクリアするということも覚えておきましょう。

.. index::
   single: Templating; Inheritance

テンプレートの継承とレイアウト
------------------------------

開発するプロジェクトの各テンプレートには、共通した要素が存在することが多くあります。\
たとえば、ヘッダーやフッター、サイドバーなどです。\
このような共通要素をインクルードするのが従来からある手法ですが、
Symfony2 では、少し異なるアプローチを提供しています。\
Symfony2 では、あるテンプレートを別のテンプレートによってデコレートできるようになっています。\
これをテンプレートの継承と呼んでおり、PHP におけるクラスの継承と同じだと考えてください。\
テンプレートの継承を使う場合、
ベーステンプレートとなる "layout" テンプレートに、サイトで使う共通要素のすべてをそれぞれ **ブロック（block）** として定義しておきます。\
これは、ベースメソッドをもっている PHP のクラスだと思ってください。\
子のテンプレート側では、layout テンプレートを継承して block をオーバーライドすることができます。\
親クラスのメソッドをサブクラスでオーバーライドすると考えて良いでしょう。

では、ベースとなる layout テンプレートを作ってみましょう。

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title>{% block title %}テストアプリケーション{% endblock %}</title>
            </head>
            <body>
                <div id="sidebar">
                    {% block sidebar %}
                        <ul>
                              <li><a href="/">ホーム</a></li>
                              <li><a href="/blog">ブログ</a></li>
                        </ul>
                    {% endblock %}
                </div>

                <div id="content">
                    {% block body %}{% endblock %}
                </div>
            </body>
        </html>

    .. code-block:: html+php

        <!-- app/Resources/views/base.html.php -->
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title><?php $view['slots']->output('title', 'テストアプリケーション') ?></title>
            </head>
            <body>
                <div id="sidebar">
                    <?php if ($view['slots']->has('sidebar')): ?>
                        <?php $view['slots']->output('sidebar') ?>
                    <?php else: ?>
                        <ul>
                            <li><a href="/">ホーム</a></li>
                            <li><a href="/blog">ブログ</a></li>
                        </ul>
                    <?php endif; ?>
                </div>

                <div id="content">
                    <?php $view['slots']->output('body') ?>
                </div>
            </body>
        </html>

.. note::

    テンプレート継承に関しては、以降では Twig でのみ説明していくことにします。\
    原理としては、Twig と PHP テンプレートで共通しています。

このテンプレートは、ベースとなるシンプルな2カラムの HTML スケルトンとなっています。\
3つの ``{% block %}`` (``title``, ``sidebar``, ``body``)が定義されており、\
各 block は、子テンプレートによってオーバーライドされるか、もしくは、デフォルトの実装のままとしておくことができます。\
このテンプレートは、そのままレンダリング可能です。\
その場合、3つの block の値は、単にこのテンプレートに記述されているデフォルト値のままとなります。

子のテンプレート側は、次のように記述します。

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Blog/index.html.twig #}
        {% extends '::base.html.twig' %}

        {% block title %}ブログ記事の一覧{% endblock %}

        {% block body %}
            {% for entry in blog_entries %}
                <h2>{{ entry.title }}</h2>
                <p>{{ entry.body }}</p>
            {% endfor %}
        {% endblock %}

    .. code-block:: html+php

        <!-- src/Acme/BlogBundle/Resources/views/Blog/index.html.php -->
        <?php $view->extend('::base.html.php') ?>

        <?php $view['slots']->set('title', 'ブログ記事の一覧') ?>

        <?php $view['slots']->start('body') ?>
            <?php foreach ($blog_entries as $entry): ?>
                <h2><?php echo $entry->getTitle() ?></h2>
                <p><?php echo $entry->getBody() ?></p>
            <?php endforeach; ?>
        <?php $view['slots']->stop() ?>

.. note::

   継承元の親テンプレートを指定するには、特別な構文を使います。
   ``::base.html.twig`` のようにバンドル名とコントローラー名を使わずに記述した場合は、\ ``app/Resources/views`` ディレクトリにあるテンプレートを指します。\
   命名規約に関しては、\ :ref:`template-naming-locations` を参照してください。

テンプレート継承は ``{% extends %}`` タグで指定します。\
このタグで、レイアウトや block が記述されているベーステンプレートを先に評価するよう、テンプレートエンジンに指示します。\
その後、子のテンプレートがレンダリングされる際、\
親テンプレートで定義された ``title`` ブロックや ``body`` ブロックが、子テンプレートの定義により置き換えられます。\
``blog_entries`` の中身にもよりますが、たとえば次のようなレンダリング結果になります。

.. code-block:: html

    <!DOCTYPE html>
    <html>
        <head>
            <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
            <title>My cool blog posts</title>
        </head>
        <body>
            <div id="sidebar">
                <ul>
                    <li><a href="/">ホーム</a></li>
                    <li><a href="/blog">ブログ</a></li>
                </ul>
            </div>

            <div id="content">
                <h2>最初の記事</h2>
                <p>最初の記事の本文です。</p>

                <h2>別の記事</h2>
                <p>2 つめの記事の本文です。</p>
            </div>
        </body>
    </html>

子テンプレートでは ``sidebar`` ブロックを定義していないので、\
親テンプレートの定義が使われます。\
親テンプレートの ``{% block %}`` 内の値が、常にデフォルトとして使用されます。

継承は好きなだけ行うことができます。\
次の章では、よくある 3 階層の継承を行うモデルを見ていきます。\
そこで、Symfony2 プロジェクト内で、どうやってテンプレートを構成していけば良いのか説明します。

テンプレート継承を行う際の Tips を紹介します。

* \ ``{% extends %}`` は、テンプレート中で一番最初のタグである必要があります。

* ベーステンプレート内では、\ ``{% block %}`` を使えば使うほどベターです。\
  親テンプレートにあるブロックに対して、対応する定義を子テンプレートにすべて記述する必要はありません。\
  ベーステンプレートに好きなだけブロックを作り、適切なデフォルト値を指定しておきましょう。\
  ベーステンプレートのブロックが多いほど、レイアウトが柔軟になるでしょう。

* テンプレート内に重複した内容がある場合、その内容を親テンプレートの ``{% block %}`` に移すのが良いでしょう。\
  または、新しいテンプレートを作って ``include`` するほうがよい場合もあります(:ref:`including-templates`\ を参照)。

* 親ブロックの block の内容を取得したい場合は、\ ``{{ parent() }}`` 関数を使います。\
  親ブロックの出力の内容を子で完全に置き換えるのではなく、親ブロックの内容に何か追加したい場合に便利です。

    .. code-block:: html+jinja

        {% block sidebar %}
            <h3>Table of Contents</h3>

            {# ... #}

            {{ parent() }}
        {% endblock %}

.. index::
   single: Templating; Naming conventions
   single: Templating; File locations

.. _template-naming-locations:

テンプレートの命名規約
----------------------

デフォルトでは、テンプレートは次の 2 つの場所に配置されます。

* ``app/Resources/views/``: アプリケーションの ``views`` ディレクトリには、\
  アプリケーション全体に関わるベーステンプレート(アプリケーションのレイアウト) や、\
  バンドルのテンプレートをオーバーライドするテンプレート(:ref:`overriding-bundle-templates`\ を参照)を置くことができます。

* ``path/to/bundle/Resources/views/``: バンドルごとのテンプレートは、バンドルの ``Resources/views`` ディレクトリ(及びそのサブディレクトリ)にあります。
  大半のテンプレートはバンドル内に配置されるでしょう。

Symfony2 では、\ **バンドル**:**コントローラー**:**テンプレート** という構文でテンプレートを指定します。\
この構文では、同じディレクトリにある複数の種類のテンプレートを扱うことができます。

* ``AcmeBlogBundle:Blog:index.html.twig``: この構文は、あるページのテンプレートを指定する時に使います。\
  コロン(``:``)によって区切られた 3 つの部分は、それぞれ次のような意味を持ちます。

    * ``AcmeBlogBundle``: (*バンドル*) テンプレートは ``AcmeBlogBundle`` (例えば ``src/Acme/BlogBundle``)内にあるということ

    * ``Blog``: (*コントローラー*) ``Resources/views`` ディレクトリ下の ``Blog`` ディレクトリにあるということ

    * ``index.html.twig``: (*テンプレート*) ファイル名が ``index.html.twig`` であること

  ``AcmeBlogBundle`` が ``src/Acme/BlogBundle`` にあるとすると、\
  最終的なパスは、 ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig`` になります。

* ``AcmeBlogBundle::layout.html.twig``: この構文は、\ ``AcmeBlogBundle`` バンドル固有のベーステンプレートを参照する時に使います。\
  先ほどの例では ``Blog`` という単語があった、2 つ目の「コントローラー」に対応する部分が空になっています。ですので、この構文で参照しているテンプレートのパスは、\ ``AcmeBlogBundle`` バンドル内の ``Resources/views/layout.html.twig`` になります。

* ``::base.html.twig``: この構文は、アプリケーション全体のベーステンプレート/レイアウトを参照する時に使います。\
  2 つのコロン(``:``)から始まりますが、これは、\ *バンドル*\ も\ *コントローラー*\ も無いということを示します。\
  テンプレートが特定のバンドルに入っているのではなく、ルートの ``app/Resources/views/`` ディレクトリにある、ということを意味します。

:ref:`overriding-bundle-templates`\ 節では、例えば ``app/Resources/AcmeBlogBundle/views/`` ディレクトリに、\ ``AcmeBlogBundle`` バンドルにあるものと同じ名前のファイルを置くことで、テンプレートをオーバーライドする方法を見ていきます。\
この方法を使うと、どんなベンダーバンドルのテンプレートでもオーバーライドできるようになります。

.. tip::

    テンプレートの命名規則にはなじみがあると思います。\
    :ref:`controller-string-syntax` で言及したものと同じ命名規則です。

サフィックス
~~~~~~~~~~~~

**バンドル**:**コントローラー**:**テンプレート** のフォーマットで、各ファイルが\ *どこに*\ 置いてあるのか指定できました。\
テンプレート名には、2 つの拡張子が付いていますが、それらは、\ *フォーマット*\ と\ *エンジン*\ を示しています。

* **AcmeBlogBundle:Blog:index.html.twig** - HTML フォーマット、Twig エンジン

* **AcmeBlogBundle:Blog:index.html.php** - HTML フォーマット、PHP エンジン

* **AcmeBlogBundle:Blog:index.css.twig** - CSS フォーマット、Twig エンジン

デフォルトでは、Symfony2 テンプレートには、Twig と PHP のどちらも使えます。\
テンプレートファイルの後ろの拡張子(``.twig`` や ``.php``)で、どのテンプレートエンジンを使うかを指定しています。\
始めの拡張子(``.html``\ 、\ ``.css``\ 、その他)は、最終的なフォーマットを示します。\
フォーマットは、同じリソースを HTML (``index.html.twig``) や XML (``index.xml.twig``)、その他複数のフォーマットでレンダリングする必要がある場合に、テンプレートを区別するのに使われます。\
詳しくは、\ :ref:`template-formats`\ を参照してください。

.. note::

   「エンジン」の有効/無効は設定可能ですし、新しいエンジンを追加することもできます。\
   :ref:`templating サービスのコンフィギュレーション <template-configuration>` を参照してください。

.. index::
   single: Templating; Tags and helpers
   single: Templating; Helpers

タグとヘルパー
--------------

命名方法や継承など、テンプレートの基本は理解できたと思いますが、一番難しい部分はこれからです。\
この節では、テンプレートのインクルードや、リンク、画像のインクルードなど、\
よくあるタスクをこなしていく際に利用可能なツールについて解説します。

Symfony2 には、テンプレートデザイナーの仕事を楽にするための Twig タグや関数がいくつか組み込まれています。\
PHP テンプレートは、拡張可能な\ *ヘルパー*\ システムを備えており、\
テンプレートコンテキストで便利な機能を提供しています。

すでに、組み込みの Twig タグ(``{% block %}``\ 、\ ``{% extends %}``)や、\
PHP ヘルパー(``$view['slots']``) をいくつか見てきました。\
もういくつかマスターしていきましょう。

.. index::
   single: Templating; Including other templates

.. _including-templates:

テンプレートをインクルードする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

同じテンプレートやコードを、別のページでインクルードしたいことはよくあります。\
たとえば、「ニュース記事」があるようなアプリケーションの場合、\
記事を表示するテンプレートコードは、記事詳細ページや、\
一番人気の記事を表示するページ、最新記事リストのページでも使用されると思います。

PHP のコードを書いている場合、再利用したいコードブロックが出てくると、\
クラスや関数を作ってそこにコードを移動させるということはよくあります。\
テンプレートの場合も同様です。\
新しくテンプレートを作成して、再利用したいテンプレートコードをそこに移動させます。\
そうすることで、テンプレートは他のテンプレートからインクルード可能になります。\
まずは、再利用したい部分のテンプレートを作成します。

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/views/Article/articleDetails.html.twig #}
        <h2>{{ article.title }}</h2>
        <h3 class="byline">by {{ article.authorName }}</h3>

        <p>
            {{ article.body }}
        </p>

    .. code-block:: html+php

        <!-- src/Acme/ArticleBundle/Resources/views/Article/articleDetails.html.php -->
        <h2><?php echo $article->getTitle() ?></h2>
        <h3 class="byline">by <?php echo $article->getAuthorName() ?></h3>

        <p>
            <?php echo $article->getBody() ?>
        </p>

他のテンプレートからインクルードするのは簡単です。

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/views/Article/list.html.twig #}
        {% extends 'AcmeArticleBundle::layout.html.twig' %}

        {% block body %}
            <h1>最新の記事<h1>

            {% for article in articles %}
                {{ include(
                    'AcmeArticleBundle:Article:articleDetails.html.twig',
                    { 'article': article }
                ) }}
            {% endfor %}
        {% endblock %}

    .. code-block:: html+php

        <!-- src/Acme/ArticleBundle/Resources/Article/list.html.php -->
        <?php $view->extend('AcmeArticleBundle::layout.html.php') ?>

        <?php $view['slots']->start('body') ?>
            <h1>最新の記事</h1>

            <?php foreach ($articles as $article): ?>
                <?php echo $view->render(
                    'AcmeArticleBundle:Article:articleDetails.html.php',
                    array('article' => $article)
                ) ?>
            <?php endforeach; ?>
        <?php $view['slots']->stop() ?>

テンプレートをインクルードするには、\ ``{% include() %}`` 関数を使います。\
インクルードするテンプレートを指定する部分は、テンプレート名の共通規則に従っています。\
``articleDetails.html.twig`` テンプレートでは、関数に渡した ``article`` 変数を使えます。\
しかし、このように毎回変数を渡す必要はなく、
``list.html.twig`` で使える変数は ``articleDetails.html.twig`` においても利用可能です
(\ `with_context`_ を ``false`` にしていない場合)。

.. tip::

    この ``{'article': article}`` という書き方は、Twig でハッシュ(名前付きキーの配列)を書くときの標準的な書き方です。\
    複数の要素があるときは、\ ``{'foo': foo, 'bar': bar}`` のように書きます。

.. index::
   single: Templating; Embedding action

.. _templating-embedding-controller:

コントローラーを埋め込む
~~~~~~~~~~~~~~~~~~~~~~~~

単純にテンプレートをインクルードする以上のことをしたい場合もあります。\
たとえば、レイアウトのサイドバーに 3 件の新着記事を載せたい場合を考えてみましょう。\
記事の取得は、データベースへのクエリーや重いロジックといった処理が必要で、テンプレート内でできるものでありません。

このような場合は、テンプレートにコントローラーの処理結果を埋め込めば良いのです。\
まずは、特定の数の最新記事をレンダリングするコントローラーを作成します。

.. code-block:: php

    // src/Acme/ArticleBundle/Controller/ArticleController.php

    class ArticleController extends Controller
    {
        public function recentArticlesAction($max = 3)
        {
            // データベースへのクエリーや他の処理を実行して
            // 最新の "$max" 個の記事を取得する
            $articles = ...;

            return $this->render(
                'AcmeArticleBundle:Article:recentList.html.twig',
                array('articles' => $articles)
            );
        }
    }

``recentList`` のテンプレートは単純です。

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}
        {% for article in articles %}
            <a href="/article/{{ article.slug }}">
                {{ article.title }}
            </a>
        {% endfor %}

    .. code-block:: html+php

        <!-- src/Acme/ArticleBundle/Resources/views/Article/recentList.html.php -->
        <?php foreach ($articles in $article): ?>
            <a href="/article/<?php echo $article->getSlug() ?>">
                <?php echo $article->getTitle() ?>
            </a>
        <?php endforeach; ?>

.. note::

    上記例では、楽をして URL をハードコードしています(``/article/*slug*``)。\
    これは良くないプラクティスです。次節で、これをうまくやる方法を紹介します。

コントローラーをインクルードするには、\ ``render`` 関数にコントローラー用のシンタックス( \**バンドル**:**コントローラー**:**アクション**)を使ってコントローラーの参照を指定します。

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}

        {# ... #}
        <div id="sidebar">
            {{ render(controller('AcmeArticleBundle:Article:recentArticles', {
                'max': 3
            })) }}
        </div>

    .. code-block:: html+php

        <!-- app/Resources/views/base.html.php -->

        <!-- ... -->
        <div id="sidebar">
            <?php echo $view['actions']->render(
                new ControllerReference(
                    'AcmeArticleBundle:Article:recentArticles',
                    array('max' => 3)
                )
            ) ?>
        </div>

変数が必要になる場合や、テンプレートからはアクセスできないような情報が必要になる場合は、\
コントローラーを埋め込むことを検討してみてください。\
コントローラーを使うと実行は速く、コードの構成は良いものに向かい、再利用性の向上にもつながります。
このように埋め込みを行うコントローラーやテンプレートは、可能な限り薄く保つべきです。
再利用可能な\ :doc:`サービス </book/service_container>`\ と同じです。

hinclude.js を使った非同期コンテンツ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

hinclude.js_ JavaScript ライブラリを使うと、コントローラーの出力を非同期に埋め込むことができます。
他のページやコントローラーの出力の埋め込みに ``hinclude`` のタグを使うようにするには、次のように ``render`` または ``render_hinclude`` 関数を使います。

.. configuration-block::

    .. code-block:: jinja

        {{ render_hinclude(controller('...')) }}
        {{ render_hinclude(url('...')) }}

    .. code-block:: php

        <?php echo $view['actions']->render(
            new ControllerReference('...'),
            array('renderer' => 'hinclude')
        ) ?>

        <?php echo $view['actions']->render(
            $view['router']->generate('...'),
            array('renderer' => 'hinclude')
        ) ?>

.. note::

   ページで hinclude.js_ をインクルードしておく必要があります。

.. note::

    URL の代わりにコントローラーを使う場合は、次のように Symfony のコンフィギュレーションで ``fragments`` を有効化しておく必要があります。

    .. configuration-block::

        .. code-block:: yaml

            # app/config/config.yml
            framework:
                # ...
                fragments: { path: /_fragment }

        .. code-block:: xml

            <!-- app/config/config.xml -->
            <?xml version="1.0" encoding="UTF-8" ?>
            <container xmlns="http://symfony.com/schema/dic/services"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xmlns:framework="http://symfony.com/schema/dic/symfony"
                xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                    http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

                <framework:config>
                    <framework:fragments path="/_fragment" />
                </framework:config>
            </container>

        .. code-block:: php

            // app/config/config.php
            $container->loadFromExtension('framework', array(
                // ...
                'fragments' => array('path' => '/_fragment'),
            ));

ロード中や JavaScript が無効にされていた場合に表示するデフォルトコンテンツを、アプリケーションコンフィギュレーションでグローバルに設定することができます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating:
                hinclude_default_template: AcmeDemoBundle::hinclude.html.twig

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config>
                <framework:templating
                    hinclude-default-template="AcmeDemoBundle::hinclude.html.twig" />
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'templating'      => array(
                'hinclude_default_template' => array(
                    'AcmeDemoBundle::hinclude.html.twig',
                ),
            ),
        ));

デフォルトコンテンツを ``render`` 関数ごとに指定することもできます。
この場合、グローバルのデフォルトテンプレートが上書きされます。

.. configuration-block::

    .. code-block:: jinja

        {{ render_hinclude(controller('...'),  {
            'default': 'AcmeDemoBundle:Default:content.html.twig'
        }) }}

    .. code-block:: php

        <?php echo $view['actions']->render(
            new ControllerReference('...'),
            array(
                'renderer' => 'hinclude',
                'default' => 'AcmeDemoBundle:Default:content.html.twig',
            )
        ) ?>

任意の文字列をデフォルトコンテンツとして指定することもできます。

.. configuration-block::

    .. code-block:: jinja

        {{ render_hinclude(controller('...'), {'default': 'Loading...'}) }}

    .. code-block:: php

        <?php echo $view['actions']->render(
            new ControllerReference('...'),
            array(
                'renderer' => 'hinclude',
                'default' => 'Loading...',
            )
        ) ?>

.. _book-templating-pages:

ページ間をリンクする
~~~~~~~~~~~~~~~~~~~~

URL については、ハードコードするのではなく、Twig の ``path`` 関数 (PHP では ``router`` ヘルパー) を使用して、\
ルーティング設定に基づいた生成を行って下さい。\
後に URL を変更したくなったときに、ルーティング設定を変更するだけですむようになります。\
テンプレート側では、新しい URL が自動的に生成されるのです。

では、"_welcome" というページにリンクしてみましょう。\
このページは、次のようなルーティング設定を通じてアクセスできるようになっています。

.. configuration-block::

    .. code-block:: yaml

        _welcome:
            path:     /
            defaults: { _controller: AcmeDemoBundle:Welcome:index }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="_welcome" path="/">
                <default key="_controller">AcmeDemoBundle:Welcome:index</default>
            </route>
        </routes>

    .. code-block:: php

        $collection = new RouteCollection();
        $collection->add('_welcome', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Welcome:index',
        )));

        return $collection;

Twig 関数である ``path`` を使用し、ルートを参照します。

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('_welcome') }}">Home</a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('_welcome') ?>">Home</a>

ご想像通り、\ ``/`` という URL が生成されます。\
もっと複雑なルートではどうなるでしょうか。

.. configuration-block::

    .. code-block:: yaml

        article_show:
            path:     /article/{slug}
            defaults: { _controller: AcmeArticleBundle:Article:show }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="article_show" path="/article/{slug}">
                <default key="_controller">AcmeArticleBundle:Article:show</default>
            </route>
        </routes>

    .. code-block:: php

        $collection = new RouteCollection();
        $collection->add('article_show', new Route('/article/{slug}', array(
            '_controller' => 'AcmeArticleBundle:Article:show',
        )));

        return $collection;

この例では、ルート名 (``article_show``) と、パラメータ ``{slug}`` の値を指定する必要があります。\
前節で扱ったテンプレート ``recentList`` を再考して、記事へ正しくリンクしてみることにしましょう。

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}
        {% for article in articles %}
            <a href="{{ path('article_show', {'slug': article.slug}) }}">
                {{ article.title }}
            </a>
        {% endfor %}

    .. code-block:: html+php

        <!-- src/Acme/ArticleBundle/Resources/views/Article/recentList.html.php -->
        <?php foreach ($articles in $article): ?>
            <a href="<?php echo $view['router']->generate('article_show', array(
                'slug' => $article->getSlug(),
            )) ?>">
                <?php echo $article->getTitle() ?>
            </a>
        <?php endforeach; ?>

.. tip::

    絶対 URL を生成することもできます。Twig 関数の ``url`` を使用します。

    .. code-block:: html+jinja

        <a href="{{ url('_welcome') }}">Home</a>

    PHP テンプレートの場合は、\ ``generate()`` メソッドに 3 番目の引数を渡します。

    .. code-block:: html+php

        <a href="<?php echo $view['router']->generate(
            '_welcome',
            array(),
            true
        ) ?>">Home</a>

.. index::
   single: Templating; Linking to assets

.. _book-templating-assets:

アセットへのリンク
~~~~~~~~~~~~~~~~~~

テンプレートでは、画像、JavaScript、スタイルシートやその他アセットを参照することもよくあります。\
もちろんハードコード(``/images/logo.png``)するのもありですが、\
Symfony2 では Twig の ``asset`` 関数でアセットのパスを生成できます。\

.. configuration-block::

    .. code-block:: html+jinja

        <img src="{{ asset('images/logo.png') }}" alt="Symfony!" />

        <link href="{{ asset('css/blog.css') }}" rel="stylesheet" type="text/css" />

    .. code-block:: html+php

        <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" alt="Symfony!" />

        <link href="<?php echo $view['assets']->getUrl('css/blog.css') ?>" rel="stylesheet" type="text/css" />

``asset`` 関数を使う一番の目的は、アプリケーションをよりポータブルにすることです。\
アプリケーションが、ホストのルート(http://example.com)に配置されていた場合、\
レンダリングされるパスは、\ ``/images/logo.png`` になっているべきです。\
では、ルートではなくサブディレクトリ(http://example.com/my_app)に配置されている場合はどうでしょう。\
アセットパスは、サブディレクトリ付き(``/my_app/images/logo.png``)で出力されなければいけません。\
``asset`` 関数は、アプリケーションがどのように使われているかをみて、この点をケアし、\
それに応じて適切なパスを生成します。

また、\ ``asset`` 関数を使うと、アセットの URL に自動的にクエリーストリングを追加できるので、デプロイ時に静的なアセットのキャッシュが残らず強制的に更新されるようにできます。たとえば、\ ``/images/logo.png`` という URL の場合は ``/images/logo.png?v2`` となります。詳細は、\ :ref:`ref-framework-assets-version` コンフィギュレーションオプションを参照してください。

.. index::
   single: Templating; Including stylesheets and JavaScripts
   single: Stylesheets; Including stylesheets
   single: Javascripts; Including JavaScripts

Twig でスタイルシートや JavaScript をインクルード
-------------------------------------------------

JavaScript や CSS をインクルードすること無く成り立っているサイトはないでしょう。\
Symfony では、これらアセットのインクルードを、Symfony テンプレート継承の利点を使って、エレガントに扱います。

.. tip::

    この節では、Symfony におけるスタイルシートや JavaScirpt のインクルードの背景にあるフィロソフィーを紹介します。\
    Symfony には、アセットを便利に扱うための Assetic というライブラリが同梱されています。
    Assetic に関するより詳しい情報は、\ :doc:`/cookbook/assetic/asset_management`\ を参照してください。

アセットが含まれたベーステンプレートに、2 つの block を追加してみましょう。\
1 つは、\ ``stylesheets`` で ``head`` タグ内に追加します。\
もう 1 つは ``javascript`` で、\ ``body`` 閉じタグの直前に追加します。\
これらの block で、サイトを通じて必要なすべてのスタイルシートや JavaScript を収容することになります。

.. code-block:: html+jinja

    {# app/Resources/views/base.html.twig #}
    <html>
        <head>
            {# ... #}

            {% block stylesheets %}
                <link href="{{ asset('/css/main.css') }}" rel="stylesheet" />
            {% endblock %}
        </head>
        <body>
            {# ... #}

            {% block javascripts %}
                <script src="{{ asset('/js/main.js') }}"></script>
            {% endblock %}
        </body>
    </html>

とても簡単ですね！\
では、子テンプレートでスタイルシートや JavaScript を追加でインクルードしたいときはどうしましょうか。\
例えば、お問い合わせページがあって、\ *そのページでだけ*  ``contact.css`` を追加でインクルードしたいとしましょう。\
お問い合わせページでは次のようにします。

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Contact/contact.html.twig #}
    {% extends '::base.html.twig' %}

    {% block stylesheets %}
        {{ parent() }}

        <link href="{{ asset('/css/contact.css') }}" rel="stylesheet" />
    {% endblock %}

    {# ... #}

子テンプレートでは、単に ``stylesheets`` をオーバーライドし、新しいスタイルシートタグを置きます。\
とはいえ、親の block の内容に追加したい (そして、\ *置き換え*\ たいわけではない) ので、\
Twig 関数の ``parent()`` を使い、ベーステンプレートの ``stylesheets`` 内のすべてをインクルードします。

また、バンドルの ``Resources/public`` フォルダに配置したアセットをインクルードすることもできます。
この場合、\ ``php app/console assets:install target [--symlink]``
コマンドを実行して、アセットファイルを Web から参照できる正しい位置（デフォルトでは "web" ディレクトリ）へコピーまたはシンボリックリンクする必要があります。

.. code-block:: html+jinja

   <link href="{{ asset('bundles/acmedemo/css/contact.css') }}" rel="stylesheet" />

この結果、\ ``main.css`` と ``contact.css`` のスタイルシートの両方共をインクルードしたページとなります。

グローバルテンプレート変数
--------------------------

Symfony2 では、テンプレートエンジンに Twig と PHP のどちらを利用していても、\ ``app`` というグローバルなテンプレート変数がリクエストごとに登録されます。
``app`` 変数は :class:`Symfony\\Bundle\\FrameworkBundle\\Templating\\GlobalVariables` クラスのインスタンスで、これを使うと次のようなアプリケーション変数へアクセスできます。

* ``app.security`` - セキュリティコンテキスト
* ``app.user`` - 現在のユーザーオブジェクト
* ``app.request`` - リクエストオブジェクト
* ``app.session`` - セッションオブジェクト
* ``app.environment`` - 現在の環境 (dev、prod など)
* ``app.debug`` - デバッグモードであれば ``true``\ 、それ以外なら ``false``

.. configuration-block::

    .. code-block:: html+jinja

        <p>ユーザー名: {{ app.user.username }}</p>
        {% if app.debug %}
            <p>リクエストメソッド: {{ app.request.method }}</p>
            <p>アプリケーション環境: {{ app.environment }}</p>
        {% endif %}

    .. code-block:: html+php

        <p>ユーザー名: <?php echo $app->getUser()->getUsername() ?></p>
        <?php if ($app->getDebug()): ?>
            <p>リクエストメソッド: <?php echo $app->getRequest()->getMethod() ?></p>
            <p>アプリケーション環境: <?php echo $app->getEnvironment() ?></p>
        <?php endif; ?>

.. tip::

    独自のグローバルテンプレート変数を追加することもできます。
    クックブックの\ :doc:`グローバル変数 </cookbook/templating/global_variables>`\ を参照してください。

.. index::
   single: Templating; The templating service

\ ``templating`` サービスの設定と使用
-------------------------------------

Symfony2 におけるテンプレートシステムの心臓部は、テンプレート ``Engine`` です。\
テンプレートをレンダリングして、その内容を返す、特別なオブジェクトです。\
たとえば、コントローラー内でテンプレートをレンダリングするときは、\
実際には、テンプレートエンジンサービスを使用しているのです。

.. code-block:: php

    return $this->render('AcmeArticleBundle:Article:index.html.twig');

上記は、次のものと同等です。

.. code-block:: php

    use Symfony\Component\HttpFoundation\Response;

    $engine = $this->container->get('templating');
    $content = $engine->render('AcmeArticleBundle:Article:index.html.twig');

    return $response = new Response($content);

.. _template-configuration:

テンプレートエンジン (もしくは「サービス」) は、Symfony2 内部であらかじめ自動的に設定済みです。\
もちろん、アプリケーションの設定ファイルで、さらなる設定が可能です。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating: { engines: ['twig'] }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config>
                <framework:templating>
                    <framework:engine id="twig" />
                </framework:templating>
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...

            'templating' => array(
                'engines' => array('twig'),
            ),
        ));

``templating`` サービスの設定オプションについては、\ :doc:`コンフィギュレーションのリファレンス </reference/configuration/framework>`\ で説明しています。

.. note::

   ``twig`` エンジンは、webprofiler (その他のサードパーティ製バンドルも) では必須となります。

.. index::
    single; Template; Overriding templates

.. _overriding-bundle-templates:

バンドルテンプレートの継承
--------------------------

Symfony2 コミュニティでは、多数の機能豊富なバンドルが開発され公開されています (`KnpBundles.com`_ 参照)。\
サードパーティ製のバンドルを使うときに、バンドルのテンプレートをオーバライドしたりカスタマイズする必要があるかもしれません。

たとえば、\ ``AcmeBlogBundle`` というオープンソースなバンドルを、\
自分のプロジェクト (``src/Acme/BlogBundle`` にあるとします) にインクルードした場合を考えてみます。\
とても満足してはいるのだけれども、1 点だけ、ブログの「リスト」ページのマークアップを自分のプロジェクトに合うように変えたいとしましょう。\
``AcmeBlogBundle`` 内のコントローラー ``Blog`` をよく見てみると、次のようなコードが見つかりました。::

    public function indexAction()
    {
        // ブログ記事一覧を取得する何らかのロジック
        $blogs = ...;

        $this->render(
            'AcmeBlogBundle:Blog:index.html.twig',
            array('blogs' => $blogs)
        );
    }

``AcmeBlogBundle:Blog:index.html.twig`` がレンダリングされる際、\
Symfony2 は、次の 2 つの場所からテンプレートを探しています。

#. ``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig``

このバンドル内テンプレートをオーバーライドするには、\ ``index.html.twig`` を\
``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig`` にコピーします\
(``app/Resources/AcmeBlogBundle`` ディレクトリが無い場合は、作成してください)。\
その後、コピーしたテンプレートを自由にカスタマイズすればよいのです。

.. caution::

    テンプレートを新しい場所に保存した場合は、デバッグモードの場合でもキャッシュクリア(``php app/console cache:clear``)が必要な場合があることに注意してください。

このロジックはバンドルのベーステンプレートにも当てはまります。\
``AcmeBlogBundle`` 内のテンプレートが、ベーステンプレート ``AcmeBlogBundle::layout.html.twig`` を継承している場合、\
先程の例と同様に、Symfony2 は次の 2 つの場所を探します。

#. ``app/Resources/AcmeBlogBundle/views/layout.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/layout.html.twig``

オーバライドするには、前と同じように、\ ``app/Resources/AcmeBlogBundle/views/layout.html.twig`` にコピーします。\
そして、見た目が合うようにこれをカスタマイズしていきます。

俯瞰してみると、Symfony2 がテンプレートを探すときは、必ず最初に ``app/Resources/{BUNDLE_NAME}/views/`` から探しているのがわかります。\
そこに何もなければ、続いてバンドルの ``Resources/views`` をチェックします。\
ですので、\ ``app/Resources`` 下に正しい構造でテンプレートを置いてやれば、任意のバンドルテンプレートをオーバーライドできます。

.. note::

    バンドルの継承機能を使った継承先のバンドルにあるテンプレートも上書きできます。
    :doc:`/cookbook/bundles/inheritance` を参照してください。

.. _templating-overriding-core-templates:

.. index::
    single; Template; Overriding exception templates

コアテンプレートをオーバーライドする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 フレームワーク自体もバンドルですので、コアテンプレートも同様にオーバーライドできます。\
TwigBundle は、内部に "exception" や "error" 用のテンプレートを多数持っていますが、\
これらも、TwigBundle バンドルの ``Resources/views/Exception`` ディレクトリから ``app/Resources/TwigBundle/views/Exception`` ディレクトリへコピーしてカスタマイズできます。

.. index::
   single: Templating; Three-level inheritance pattern

3 階層の継承
------------

継承を使う際によくあるのが、3 階層のアプローチです。\
このアプローチは、今まで見てきた次の 3 種のテンプレートからなります。

* ``app/Resources/views/base.html.twig`` を作成します。\
  これには、(先程の例のような) アプリケーションのメインレイアウトをいれます。\
  内部的には、\ ``::base.html.twig`` となります。

* サイトの各「セクション」毎のテンプレートを作成します。\
  ``AcmeBlogBundle`` であれば、\ ``AcmeBlogBundle::layout.html.twig`` でしょう。\
  そこに、ブログ用の要素だけをいれます。

    .. code-block:: html+jinja

      {# src/Acme/BlogBundle/Resources/views/layout.html.twig #}
      {% extends '::base.html.twig' %}

      {% block body %}
          <h1>Blog Application</h1>

          {% block content %}{% endblock %}
      {% endblock %}

* 各ページ専用のテンプレートを作成します。\
  それぞれ、適切なセクションのテンプレートを継承してください。\
  "index" ページであれば、\ ``AcmeBlogBundle:Blog:index.html.twig`` のようなファイルに、ブログエントリをリストすればよいでしょう。

    .. code-block:: html+jinja

      {# src/Acme/BlogBundle/Resources/views/Blog/index.html.twig #}
      {% extends 'AcmeBlogBundle::layout.html.twig' %}

      {% block content %}
          {% for entry in blog_entries %}
              <h2>{{ entry.title }}</h2>
              <p>{{ entry.body }}</p>
          {% endfor %}
      {% endblock %}

各ページのテンプレートはセクションテンプレート (``AcmeBlogBundle::layout.html.twig``) を継承し、\
セクションテンプレートはアプリケーションレイアウト (``::base.html.twig``) を継承しています。\
これが、3 階層継承というものです。

アプリケーションを作るときは、たいてい、この 3 階層継承を使うか、もしくは、\
各ページテンプレートがアプリケーションテンプレートを直接継承する (たとえば ``{% extends '::base.html.twig' %}``) ことになるでしょう。\
3 階層モデルは、ベンダーバンドルにより使用されるベストプラクティスで、\
バンドルのベーステンプレートが、アプリケーションのベースレイアウトを適切に継承するように、\
簡単にオーバーライドできます。

.. index::
   single: Templating; Output escaping

アウトプットエスケープ
----------------------

テンプレートから HTML を生成する際に、常にリスクになるのが、\
意図しない HTML や、危険なクライアントサイドコードを出力してしまうテンプレート変数の存在です。\
結果的に、動的なコンテンツが HTML を壊したり、悪意あるユーザーによる `クロスサイトスクリプティング`_ (XSS) 攻撃を許してしまいます。\
その典型例を見ていきましょう。

.. configuration-block::

    .. code-block:: jinja

        Hello {{ name }}

    .. code-block:: html+php

        Hello <?php echo $name ?>

もしユーザーが次のような名前を入力したとしたらどうでしょうか。:

.. code-block:: html

    <script>alert('hello!')</script>

アウトプットエスケープをしなければ、このテンプレートでは、\
JavaScript のアラートボックスがポップアップしてしまうでしょう。

.. code-block:: html

    Hello <script>alert('hello!')</script>

上のスクリプトでは問題がないように見えますが、\
ここまでできてしまうことを知ったユーザーに悪意があれば、\
何も知らない正当なユーザーのセキュアな場所で悪さをするJavaScriptを書けてしまいます。

この問題への答えは、アウトプットエスケープです。\
これが有効になっていれば、テンプレートは害を及ぼさない形でレンダリングされ、\
``script`` とそのまま文字通り画面に出力されます。

.. code-block:: html

    Hello &lt;script&gt;alert(&#39;helloe&#39;)&lt;/script&gt;

Twig と PHP では、異なるアプローチをとっています。\
Twig の場合であれば、アウトプットエスケープは常に on になっているので、安全です。\
PHP の場合は自動とはいかず、必要な場合には常に手動でエスケープする必要があります。

Twig の場合
~~~~~~~~~~~

Twig テンプレートを使っていれば、アウトプットエスケープはデフォルトで有効です。\
従って、ユーザーが送信したコードによる意図しない挙動を回避する、という意味ではデフォルトで安全です。\
デフォルトで、コンテンツの HTML 出力はエスケープされるべきとされているのです。

アウトプットエスケープを無効にして、信用に足る変数のレンダリングや、\
マークアップが含まれていてエスケープすべきでない変数をレンダリングする場合もあるでしょう。\
管理者が HTML コードを含む記事を書く場合を考えてみましょう。\
Twig では、デフォルトで記事をエスケープしてしまいます。

通常は ``raw`` フィルターを追加して、レンダーします。

.. code-block:: jinja

    {{ article.body|raw }}

``{% block %}`` 単位、もしくはテンプレート全体でアウトプットエスケープを無効にすることもできます。\
詳細は、Twig ドキュメントの `アウトプットエスケープ`_ を参照してください。

PHP の場合
~~~~~~~~~~

PHP テンプレートを使用している場合は、自動的なアウトプットエスケープは行われません。\
つまり、明示的に変数をエスケープしなければ、安全でなくなります。\
エスケープを行うには、view のメソッドである ``escape()`` を使用します。

.. code-block:: html+php

    Hello <?php echo $view->escape($name) ?>

``escape()`` メソッドは、デフォルトでは、変数は HTML コンテキストでレンダリングする想定になっています\
(したがって、HTML 的に安全になるように変数はエスケープされます)。\
2 つ目の引数でコンテキストを変更できます。\
例えば、JavaScript 内で何か出力したいときは、\ ``js`` コンテキストを使用します。

.. code-block:: html+php

    var myMsg = 'Hello <?php echo $view->escape($name, 'js') ?>';

.. index::
   single: Templating; Formats

デバッグ
--------

PHP では ``var_dump()`` 関数を使って渡された変数の中身を手軽にチェックできます。
コントローラー内では、この関数を使ってデバッグできます。
Twig テンプレートでは、debug エクステンションの機能を使います。

``dump`` 関数を使って、テンプレート変数をデバッグできます。

.. code-block:: html+jinja

    {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}
    {{ dump(articles) }}

    {% for article in articles %}
        <a href="/article/{{ article.slug }}">
            {{ article.title }}
        </a>
    {% endfor %}

``config.yml`` において Twig の ``debug`` 設定が ``true`` の場合のみ、ダンプが出力されます。
デフォルトでは、\ ``dev`` 環境においてのみダンプが有効で、\ ``prod`` 環境では無効になっています。

構文チェック
------------

コンソールコマンドの ``twig:lint`` を使って、Twig テンプレートの構文エラーをチェックすることができます。

.. code-block:: bash

    # ファイル名を指定してチェック:
    $ php app/console twig:lint src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig

    # ディレクトリ単位でチェック:
    $ php app/console twig:lint src/Acme/ArticleBundle/Resources/views

    # バンドル単位でチェック:
    $ php app/console twig:lint @AcmeArticleBundle

.. _template-formats:

テンプレートフォーマット
------------------------

テンプレートは、\ *あらゆる*\ フォーマットでコンテンツをレンダリングするための方法です。\
ほとんどは HTML コンテンツをレンダリングするのにテンプレートを使うことになるでしょうが、\
JavaScript や CSS、XML、その他考えうるフォーマットでも簡単に生成することもできます。

たとえば、同一の「リソース」でも、複数のフォーマットでレンダリングされることはよくあります。\
index ページを XML でレンダリングしたいときは、レンダリングするテンプレートのフォーマット部分を変更します。

* *XML テンプレート名*: ``AcmeArticleBundle:Article:index.xml.twig``
* *XML テンプレートファイル名*: ``index.xml.twig``

これは命名規則以外の何者でもなく、\
この段階ではフォーマットに基づいたテンプレートレンダリングがされるわけではありません。

1 つのコントローラーで、「リクエストフォーマット」に応じて複数のフォーマットでレンダリングしたい場合も多くあると思います。\
次のようにするのが一般的でしょう。

.. code-block:: php

    public function indexAction()
    {
        $format = $this->getRequest()->getRequestFormat();

        return $this->render('AcmeBlogBundle:Blog:index.'.$format.'.twig');
    }

``Request`` オブジェクトの ``getRequestFormat()`` メソッドは、デフォルトでは ``html`` を返しますが、\
ユーザーによりリクエストされたフォーマットが異なれば、それが返されます。\
リクエストフォーマットは、ほとんどの場合、ルーティングによって扱われます。\
たとえば、\ ``/contact`` であれば ``html``\ 、\ ``contact.xml``\ であれば ``xml`` のように設定できます。\
詳細は、\ :ref:`ルーティング <advanced-routing-example>`\ を参照してください。

フォーマットをリンクに入れたい場合は、パラメータに ``_format`` キーで指定してください。

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('article_show', {'id': 123, '_format': 'pdf'}) }}">
            PDF バージョン
        </a>

    .. code-block:: html+php

        <a href="<?php echo $view['router']->generate('article_show', array(
            'id' => 123,
            '_format' => 'pdf',
        )) ?>">
            PDF バージョン
        </a>

まとめ
------

Symfony のテンプレートエンジンは強力なツールで、\
表層的なコンテンツを、HTML や、XMLその他フォーマットで生成したい場合に使うツールです。\
テンプレートは、コントローラー内でコンテンツを生成する一般的な方法ですが、\
必須というわけではありません。\
テンプレート使わなくても、次のように ``Response`` オブジェクトを作成してコントローラーから返すことはできます。

.. code-block:: php

    // レンダリングされたテンプレートをコンテンツとする Response オブジェクト
    $response = $this->render('AcmeArticleBundle:Article:index.html.twig');

    // 単純なテキストをコンテンツとする Response オブジェクト
    $response = new Response('response content');

Symfony のテンプレートエンジンはとても柔軟で、デフォルトでは 2 種類のレンダラーが利用可能です。\
*PHP* テンプレートと、\ *Twig* です。\
両者とも、階層的なテンプレートをサポートしており、\
一般的なタスクのほとんどをこなすことのできるヘルパー関数が豊富にパッケージされています。

全体として、テンプレートの主題は、自由に使える強力なツールであると考えられるべきです。\
テンプレートをレンダリングする必要のない場合もあるかもしれませんが、\
Symfony2 では、その場合でも問題ありません。

Cookbook でもっと学ぶ
---------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/error_pages`
* :doc:`/cookbook/templating/twig_extension`

.. _`Twig`: http://twig.sensiolabs.org
.. _`KnpBundles.com`: http://knpbundles.com
.. _`クロスサイトスクリプティング`: http://en.wikipedia.org/wiki/Cross-site_scripting
.. _`アウトプットエスケープ`: http://twig.sensiolabs.org/doc/api.html#escaper-extension
.. _`タグ`: http://twig.sensiolabs.org/doc/tags/index.html
.. _`フィルター`: http://twig.sensiolabs.org/doc/filters/index.html
.. _`エクステンションを追加する`: http://twig.sensiolabs.org/doc/advanced.html#creating-an-extension
.. _`hinclude.js`: http://mnot.github.com/hinclude/
.. _`with_context`: http://twig.sensiolabs.org/doc/functions/include.html
.. _`include() 関数`: http://twig.sensiolabs.org/doc/functions/include.html
.. _`{% include %} タグ`: http://twig.sensiolabs.org/doc/tags/include.html

.. 2011/08/08 gilbite d7f118ff2c3f5fb73f1d2be27d2c88f166fbc10d
.. 2011/12/28 hidenorigoto 5034f36ebb0abe7aa86bb8d90f3a3454fb28e8b2
.. 2013/12/31 hidenorigoto 1401372c5450613c99396ad61dd9e7d499831f5c 
