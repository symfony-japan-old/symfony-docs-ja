.. index::
   single: Templating

テンプレートの基本
==================

ご存知のとおり、\ :doc:`コントローラ </book/controller>` は、\
Symfony2 アプリケーションに入ってきたリクエストを扱う役割を果たします。\
ただし、実際は、コードのテストのしやすさや再利用性のために、重い処理を別の部分に任せていることもあります。\
コントローラは、HTML や CSS その他のコンテンツを生成する際は、その生成処理をテンプレートエンジンに引き継ぎます。\
本章では、ユーザに提示するコンテンツや、メール本文などのテンプレートの記述方法をマスターしていきます。\
テンプレートを継承したりコードを再利用する方法も勉強していきましょう。

.. index::
   single: Templating; What is a template?

テンプレート
------------

テンプレートとは、テキストベースのフォーマット(HTML、XML、CSV、LaTeX ...)なら何でも生成することが可能な、シンプルなテキストファイルです。\
一番身近なのは PHP テンプレートでしょう。\
テキストと PHP コードが混ざったテキストファイルが、 PHP によってパースされるものです。 

.. code-block:: html+php

    <!DOCTYPE html>
    <html>
        <head>
            <title>Welcome to Symfony!</title>
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

Symfony2 には、より強力なテンプレート言語である `Twig`_ がパッケージされています。\
Twig では、簡潔で可読性の高いテンプレートを記述することが可能で、\
WEB デザイナーにもフレンドリーです。\
そして、PHP テンプレートよりも強力な点もあります。

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html>
        <head>
            <title>Welcome to Symfony!</title>
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

Twig には2種類の構文があります。

* ``{{ ... }}``: "言う": 変数や式の結果をテンプレートに表示する

* ``{% ... %}``: "する": テンプレートのロジックを制御する\ **タグ**\で、for ループなどの命令を実行する際に使用する

.. note::

   もう一つ、次のようなコメント用の構文もあります。
   ``{# this is a comment #}``
   この構文では、PHP の ``/* comment */`` と同じように複数行に渡るコメントも記述できます。

Twig には\ **フィルタ**\ という機能もあります。フィルタを使うと、レンダリングされる前に、コンテンツを修飾することができます。\
次の例では、フィルタを使って変数 ``title`` の内容をすべて大文字に変換して出力します。

.. code-block:: jinja

    {{ title | upper }}

デフォルトで有効な\ `タグ`_\ や\ `フィルタ`_\ は数多くあります。\
また、必要であれば、自分で\ `エクステンションを追加する`_\ ことも可能です。

.. tip::

    新しく作成した Twig エクステンションを登録するには、新しいサービスを作って、\
    それに ``twig.extension`` という\ :ref:`タグ<reference-dic-tags-twig-extension>`\ を付ければよいだけなので、\
    難しくありません。

この後にも出てきますが、Twig は関数の使用をサポートしています。\
また、新しい関数を容易に追加できます。\
下の例では、デフォルトのタグ ``for``  と関数 ``cycle`` を使って、\
10個の div タグを出力しています。\
この際、div タグの class 属性として ``odd`` と ``even`` が交互に適用されます。

.. code-block:: html+jinja

    {% for i in 0..10 %}
      <div class="{{ cycle(['odd', 'even'], i) }}">
        <!-- some HTML here -->
      </div>
    {% endfor %}

この章では、テンプレートの例は Twig と PHP の両方で示していきます。

.. sidebar:: Why Twig?

    Twig テンプレートはシンプルでなければなりませんし、PHP タグを処理することはありません。\
    これは、Twig テンプレートシステムは、見た目の表現手段として作られているのであり、\
    プログラムロジックとして作られているわけではない、という設計によるものです。\
    Twig を使えば使うほど、この性質に感謝し利益が得られるでしょう。\
    そしてもちろん、あなたは、どこの WEB デザイナーからも愛される存在になるでしょう。

    PHP テンプレートではできないようなことも Twig では可能になります。例えば、\
    真のテンプレート継承(Twig テンプレートは、継承関係のついた PHP クラスにコンパイルされる) 、\
    空白字の制御、サンドボックス、テンプレート内のみで有効なカスタム関数やフィルタのインクルード、など。\
    Twig には、テンプレートの記述を容易にそして簡潔にする仕組みがいくつかあります。\
    次の例では、論理命令である ``if`` をループに一体化させています。


    .. code-block:: html+jinja

        <ul>
            {% for user in users %}
                <li>{{ user.username }}</li>
            {% else %}
                <li>No users found</li>
            {% endfor %}
        </ul>

.. index::
   pair: Twig; Cache

Twig テンプレートのキャッシュ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Twig は高速です。各テンプレートはネイティブな PHP クラスにコンパイルされ、実行時に表示されます。\
コンパイルされたクラスは、\ ``app/cache/{environment}/twig`` (``{environment}`` は ``dev`` や ``prod`` のような環境のこと) に配置されますので、\
デバッグ時に便利な場合があるかもしれません。\
環境についてより詳しく知りたければ、\ :ref:`environments-summary` を参照してください。

``debug`` モードが有効になっている場合(``dev`` 環境ではそうします)は、\
Twig テンプレートは、変更が加えられていれば自動的に再コンパイルされます。\
ですので、開発中は特にキャッシュを消す心配をすることなく、テンプレートに加えた変更を即座に確認できます。

``debug`` モードが無効の場合(``prod`` 環境ではそうします)は、\
Twig テンプレートを再生成するには Twig キャッシュディレクトリをクリアする必要があります。\
アプリケーションをデプロイするときは、必ずキャッシュをクリアするということも覚えておきましょう。

.. index::
   single: Templating; Inheritance

テンプレートの継承とレイアウト
------------------------------

開発するプロジェクト内の各テンプレートには、共通した要素が存在することが多くあります。\
たとえば、ヘッダーやフッター、サイドバーなどです。\
Symfony2 を使うのであれば、この問題を別の角度から見たいと思います。\
すなわち、あるテンプレートを、別のテンプレートによってデコレートできる、と捉えます。\
この考え方は、PHP のクラスの考えと全く同じです。\
テンプレートの継承を使う場合、
ベーステンプレートとなる "layout" テンプレート内に **ブロック（block）** として、サイトの全ての共通要素を定義することができます。\
これは、ベースメソッドをもっている PHP のクラスだと思ってください。\
子のテンプレート側では、layout テンプレートを継承して、block をオーバーライドすることができます。\
親クラスのメソッドをサブクラスでオーバーライドすると考えて良いでしょう。

では、ベースとなる layout テンプレートを作ってみましょう。

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title>{% block title %}Test Application{% endblock %}</title>
            </head>
            <body>
                <div id="sidebar">
                    {% block sidebar %}
                    <ul>
                        <li><a href="/">Home</a></li>
                        <li><a href="/blog">Blog</a></li>
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
                <title><?php $view['slots']->output('title', 'Test Application') ?></title>
            </head>
            <body>
                <div id="sidebar">
                    <?php if ($view['slots']->has('sidebar'): ?>
                        <?php $view['slots']->output('sidebar') ?>
                    <?php else: ?>
                        <ul>
                            <li><a href="/">Home</a></li>
                            <li><a href="/blog">Blog</a></li>
                        </ul>
                    <?php endif; ?>
                </div>

                <div id="content">
                    <?php $view['slots']->output('body') ?>
                </div>
            </body>
        </html>

.. note::

    テンプレート継承に関しては、今後 Twig で議論していくことにします。\
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

        {% block title %}My cool blog posts{% endblock %}

        {% block body %}
            {% for entry in blog_entries %}
                <h2>{{ entry.title }}</h2>
                <p>{{ entry.body }}</p>
            {% endfor %}
        {% endblock %}

    .. code-block:: html+php

        <!-- src/Acme/BlogBundle/Resources/views/Blog/index.html.php -->
        <?php $view->extend('::base.html.php') ?>

        <?php $view['slots']->set('title', 'My cool blog posts') ?>

        <?php $view['slots']->start('body') ?>
            <?php foreach ($blog_entries as $entry): ?>
                <h2><?php echo $entry->getTitle() ?></h2>
                <p><?php echo $entry->getBody() ?></p>
            <?php endforeach; ?>
        <?php $view['slots']->stop() ?>

.. note::

   継承元の親テンプレートを指定するには、特別な構文を使います。
   ``::base.html.twig`` と記述した場合は、\ ``app/Resources/views`` ディレクトリにあるテンプレートを指すことになります。\
   命名規約に関しては、\ :ref:`template-naming-locations` を参照してください。

テンプレート継承の鍵となるのは ``{% extends %}`` タグです。\
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
                    <li><a href="/">Home</a></li>
                    <li><a href="/blog">Blog</a></li>
                </ul>
            </div>

            <div id="content">
                <h2>My first post</h2>
                <p>The body of the first post.</p>

                <h2>Another post</h2>
                <p>The body of the second post.</p>
            </div>
        </body>
    </html>

子テンプレートでは ``sidebar`` ブロックを定義していないので、\
親テンプレートの定義が使われます。\
親テンプレートの ``{% block %}`` 内の値が、常にデフォルトとして使用されます。

継承は好きなだけ行うことができます。\
次の章では、よくある 3 階層の継承を行うモデルを見ていきます。\
そこで、Symfony2 プロジェクト内で、どうやってテンプレートを構成していけば良いのか説明します。

ここで、テンプレート継承を行う際の、心に留めておきたい Tips を紹介します。

* \ ``{% extends %}`` は、テンプレート中で一番最初のタグである必要があります。

* ベーステンプレート内では、\ ``{% block %}`` を使えば使うほどベターです。\
  親テンプレートにあるブロックに対して、子テンプレート側で対応するすべての定義を記述する必要はありません。\
  ベーステンプレートに好きなだけブロックを作り、適切なデフォルト値を指定しておきましょう。\
  ブロックが多いほど、レイアウトが柔軟になるでしょう。

* テンプレート内に重複した内容がある場合、その内容を親テンプレートの\ ``{% block %}`` に移すのが良いでしょう。\
  または、新しいテンプレートを作って ``include`` するほうがよい場合もあります(:ref:`including-templates`\ を参照)。

* 親ブロックの block の内容を取得したい場合は、\ ``{{ parent() }}`` 関数を使います。\
  親ブロックの出力の内容を子で完全に置き換えるのではなく、親ブロックの内容に何か追加したい場合に便利です。

    .. code-block:: html+jinja

        {% block sidebar %}
            <h3>Table of Contents</h3>
            ...
            {{ parent() }}
        {% endblock %}

.. index::
   single: Templating; Naming Conventions
   single: Templating; File Locations

.. _template-naming-locations:

テンプレートの命名規約
----------------------

デフォルトでは、テンプレートは次の2つの場所に配置されます。

* ``app/Resources/views/``: アプリケーションの ``views`` ディレクトリには、\
  アプリケーション全体に関わるベーステンプレート(アプリケーションのレイアウト) や、\
  バンドルのテンプレートをオーバーライドするテンプレート(:ref:`overriding-bundle-templates`\ を参照)を置くことができます。

* ``path/to/bundle/Resources/views/``: 各バンドルごとのテンプレートは、バンドルの ``Resources/views`` ディレクトリ(及びそのサブディレクトリ)にあります。
  大半のテンプレートはバンドル内に配置されるでしょう。

Symfony2 では、\ **バンドル**:**コントローラ**:**テンプレート** という構文でテンプレートを指定します。\
この構文では、同じディレクトリにある複数の種類のテンプレートを扱うことができます。

* ``AcmeBlogBundle:Blog:index.html.twig``: この構文は、あるページのテンプレートを指定する時に使います。\
  コロン(``:``)によって区切られた3つの部分は、それぞれ次のような意味を持ちます。

    * ``AcmeBlogBundle``: (*バンドル*) テンプレートは ``AcmeBlogBundle`` (例えば ``src/Acme/BlogBundle``)内にあるということ

    * ``Blog``: (*コントローラ*) ``Resources/views`` ディレクトリ下の ``Blog`` ディレクトリにあるということ

    * ``index.html.twig``: (*テンプレート*) ファイル名が ``index.html.twig`` であること

  ``AcmeBlogBundle`` が ``src/Acme/BlogBundle`` にあるとすると、\
  最終的なパスは、 ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig`` になります。


* ``AcmeBlogBundle::layout.html.twig``: この構文は、\ ``AcmeBlogBundle`` バンドル固有のベーステンプレートを参照する時に使います。\
  先ほどの例では ``Blog`` という単語があった、2 つ目の「コントローラ」に対応する部分が空になっています。ですので、この構文で参照しているテンプレートのパスは、\ ``AcmeBlogBundle`` バンドル内の ``Resources/views/layout.html.twig`` になります。

* ``::base.html.twig``: この構文は、アプリケーション全体のベーステンプレート/レイアウトを参照する時に使います。\
  2つのコロン(``:``)から始まりますが、これは、\ *バンドル*\ も\ *コントローラ*\ も無いということを示します。\
  テンプレートが特定のバンドルに入っているのではなく、ルートの ``app/Resources/views/`` ディレクトリにある、ということを意味します。

:ref:`overriding-bundle-templates` 節では、例えば ``app/Resources/AcmeBlogBundle/views/`` ディレクトリに、\ ``AcmeBlogBundle`` バンドルにあるものと同じ名前のファイルを置くことで、テンプレートをオーバーライドする方法を見ていきます。\
この方法を使うと、どんなベンダーバンドルのテンプレートでもオーバーライドできるようになります。

.. tip::

    テンプレートの命名規則にはなじみがあるとおもいます。\
    :ref:`controller-string-syntax` で言及したものと同じ命名規則です。

サフィックス
~~~~~~~~~~~~

**バンドル**:**コントローラ**:**テンプレート** のフォーマットで、各ファイルが\ *どこに*\ 置いてあるのか指定できました。\
テンプレート名には、2つの拡張子が付いていますが、それらは、\ *フォーマット*\ と\ *エンジン*\ を示しています。


* **AcmeBlogBundle:Blog:index.html.twig** - HTML フォーマット, Twig エンジン

* **AcmeBlogBundle:Blog:index.html.php** - HTML フォーマット, PHP エンジン

* **AcmeBlogBundle:Blog:index.css.twig** - CSS フォーマット, Twig エンジン

.. todo burshup

デフォルトでは、Symfony2 テンプレートは、Twig でも PHP でも、どちらででも書くことができます。\
後ろの拡張子(``.twig`` や ``.php``)は、そのどちらのエンジンを使うかを指定しています。\
始めの拡張子(``.html``\ 、\ ``.css``\ 、その他)は、最終的なフォーマットを示します。\
こちらは、Symfony2 がどうやってパースするのか決定するエンジンの指定部とは違って、\
同じリソースを HTML (``index.html.twig``)や、XML (``index.xml.twig``)、その他でレンダリングする必要がある際の、\
organizational tactic として使用されます。\
詳しくは、\ :ref:`template-formats`\ を参照してください。


.. note::

   「エンジン」の有効/無効は設定可能ですし、新しいエンジンを追加することもできます。\
   :ref:`Templating Configuration<template-configuration>` を参照してください。

.. index::
   single: Templating; Tags and Helpers
   single: Templating; Helpers

タグとヘルパー
--------------

命名方法や継承など、テンプレートの基本は理解できたと思いますが、一番難しい部分はこれからです。\
この節では、テンプレートのインクルードや、リンク、画像のインクルードなど、\
よくあるタスクをこなしていく際に利用可能なツールについて、たくさん見ていきたいと思います。

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

同じテンプレートやコードを、別のページでインクルードしたいことは、よくあります。\
たとえば、「ニュース記事」があるようなアプリケーションの場合、\
記事を表示するテンプレートコードは、記事詳細ページや、\
一番人気の記事を表示するページ、最新記事リストのページでも使用されると思います。

PHP のコードを書いている場合、再利用したいコードブロックが出てくると、\
クラスや関数を作ってそこにコードを移動させるということはよくあります。\
テンプレートの場合も同様です。\
新しくテンプレートを作成して、再利用したいテンプレートコードをそこに移動させるのです。\
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

        {# src/Acme/ArticleBundle/Resources/Article/list.html.twig #}
        {% extends 'AcmeArticleBundle::layout.html.twig' %}

        {% block body %}
            <h1>Recent Articles<h1>

            {% for article in articles %}
                {% include 'AcmeArticleBundle:Article:articleDetails.html.twig' with {'article': article} %}
            {% endfor %}
        {% endblock %}

    .. code-block:: html+php

        <!-- src/Acme/ArticleBundle/Resources/Article/list.html.php -->
        <?php $view->extend('AcmeArticleBundle::layout.html.php') ?>

        <?php $view['slots']->start('body') ?>
            <h1>Recent Articles</h1>

            <?php foreach ($articles as $article): ?>
                <?php echo $view->render('AcmeArticleBundle:Article:articleDetails.html.php', array('article' => $article)) ?>
            <?php endforeach; ?>
        <?php $view['slots']->stop() ?>

テンプレートをインクルードするには、\ ``{% include %}`` タグを使います。\
インクルードするテンプレートを指定する部分は、テンプレート名の共通規則に従っています。\
``articleDetails.html.twig`` テンプレートでは、変数 ``article`` を使用しますが、\
この変数は、\ ``list.html.twig`` 内で、\ ``with`` コマンドを使用して渡されます。


.. tip::

    この ``{'article': article}`` という書き方は、Twig のハッシュ(名前付きキーの配列)を書くときのスタンダードな書き方です。\
    複数の要素があるときは、\ ``{'foo': foo, 'bar': bar}`` のように書きます。

.. index::
   single: Templating; Embedding action

.. _templating-embedding-controller:

コントローラを埋め込む
~~~~~~~~~~~~~~~~~~~~~~

シンプルなテンプレートをインクルードする以上のことをしたい場合もありますよね。\
たとえば、レイアウトのサイドバーに、3件の新着記事を載せたい場合を考えてみましょう。\
記事の取得は、データベースに問い合せたりその他重いロジックを走らせたりと、テンプレート内でできるものでありません。

こういう場合は、テンプレート内からコントローラの結果を組み込めば良いのです。\
まずは、特定の数の最新記事をレンダリングするコントローラを作成します。

.. code-block:: php

    // src/Acme/ArticleBundle/Controller/ArticleController.php

    class ArticleController extends Controller
    {
        public function recentArticlesAction($max = 3)
        {
            // make a database call or other logic to get the "$max" most recent articles
            $articles = ...;

            return $this->render('AcmeArticleBundle:Article:recentList.html.twig', array('articles' => $articles));
        }
    }

テンプレート ``recentList`` は、まったくもってそのままです。

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

コントローラをインクルードするには、例のコントローラ用のシンタックス( **バンドル**:**コントローラ**:**アクション**)を使って指定します。


.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        ...

        <div id="sidebar">
            {% render "AcmeArticleBundle:Article:recentArticles" with {'max': 3} %}
        </div>

    .. code-block:: html+php

        <!-- app/Resources/views/base.html.php -->
        ...

        <div id="sidebar">
            <?php echo $view['actions']->render('AcmeArticleBundle:Article:recentArticles', array('max' => 3)) ?>
        </div>

変数が必要になってくる場合や、テンプレートからはアクセスできないような情報が必要になる場合は、\
コントローラをレンダリングすることを考慮してみてください。\
コントローラですと実行は速いですし、コードの構成は良いものに向かいますし、再利用性の向上にもつながります。

.. index::
   single: Templating; Linking to pages

ページ間をリンクする
~~~~~~~~~~~~~~~~~~~~

URL については、ハードコードするのではなくて、Twig の ``path`` 関数(PHP だと、\ ``router`` ヘルパ)を使用して、\
ルーティング設定に基づいた生成を行って下さい。\
後に URL の変更をしたくなったときに、ルーティング設定を変更するだけですむようになります。\
テンプレート側では、新しい URL が自動的に生成されるのです。

では、"_welcome" というページにリンクしてみましょう。\
このページは、次のようなルーティング設定を通じてアクセスできるようになっています。

.. configuration-block::

    .. code-block:: yaml

        _welcome:
            pattern:  /
            defaults: { _controller: AcmeDemoBundle:Welcome:index }

    .. code-block:: xml

        <route id="_welcome" pattern="/">
            <default key="_controller">AcmeDemoBundle:Welcome:index</default>
        </route>

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
            pattern:  /article/{slug}
            defaults: { _controller: AcmeArticleBundle:Article:show }

    .. code-block:: xml

        <route id="article_show" pattern="/article/{slug}">
            <default key="_controller">AcmeArticleBundle:Article:show</default>
        </route>

    .. code-block:: php

        $collection = new RouteCollection();
        $collection->add('article_show', new Route('/article/{slug}', array(
            '_controller' => 'AcmeArticleBundle:Article:show',
        )));

        return $collection;

この例では、ルート名(``article_show``)と、パラメータ ``{slug}`` の値を指定してやる必要があります。\
前節で扱ったテンプレート ``recentList`` を再考して、記事へ正しくリンクしてみることにしましょう。

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}
        {% for article in articles %}
          <a href="{{ path('article_show', { 'slug': article.slug }) }}">
              {{ article.title }}
          </a>
        {% endfor %}

    .. code-block:: html+php

        <!-- src/Acme/ArticleBundle/Resources/views/Article/recentList.html.php -->
        <?php foreach ($articles in $article): ?>
            <a href="<?php echo $view['router']->generate('article_show', array('slug' => $article->getSlug()) ?>">
                <?php echo $article->getTitle() ?>
            </a>
        <?php endforeach; ?>

.. tip::

    絶対 URL を生成することもできます。 Twig 関数の ``url`` を使用します。

    .. code-block:: html+jinja

        <a href="{{ url('_welcome') }}">Home</a>

    PHP テンプレートの場合は、\ ``generate()`` メソッドに3番目の引数を渡します。

    .. code-block:: html+php

        <a href="<?php echo $view['router']->generate('_welcome', array(), true) ?>">Home</a>

.. index::
   single: Templating; Linking to assets

アセットへのリンク
~~~~~~~~~~~~~~~~~~

テンプレートでは、画像や、Javascript、スタイルシートやその他アセットを参照することもよくあります。\
もちろんハードコード(``/images/logo.png``)するのもありでしょうが、\
Symfony2 には Twig 関数 ``asset`` を経由させる、という、より動的なオプションもあります。\

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
では、ルートではなくてサブディレクトリ(http://example.com/my_app)に配置されていた場合はどうでしょう。\
アセットパスは、サブディレクトリ付き(``/my_app/images/logo.png``)で出力されなければいけません。\
``asset`` 関数は、アプリケーションがどのように使われているかみて、この点をケアし、\
それに応じて適切なパスを生成します。

また、\ ``asset`` 関数を使うと、アセットの URL に自動的にクエリーストリングを追加できるので、デプロイ時に静的なアセットのキャッシュが残らず強制的に更新されるようにできます。たとえば、\ ``/images/logo.png`` という URL の場合は ``/images/logo.png?v2`` となります。詳細は、\ :ref:`ref-framework-assets-version` コンフィギュレーションオプションを参照してください。

.. index::
   single: Templating; Including stylesheets and Javascripts
   single: Stylesheets; Including stylesheets
   single: Javascripts; Including Javascripts

Twig でスタイルシートや Javascript をインクルード
-------------------------------------------------

Javascript や CSS をインクルードすること無く成り立っているサイトはないでしょう。\
Symfony では、これらアセットのインクルードを、Symfony テンプレート継承の利点を使って、エレガントに扱います。

.. tip::

    この節では、Symfony における、スタイルシートや Javascirpt のインクルードの背景にあるフィロソフィーを紹介します。\
    Symfony は、Assetic という、このフィロソフィに従っていて、かつ、アセットを使ったより興味深いことができるライブラリをパッケージしています。
    Assetic に関するより詳しい情報は、\ :doc:`/cookbook/assetic/asset_management`\ を参照してください。


アセットが含まれたベーステンプレートに、2つの block を追加してみましょう。\
1つは、\ ``stylesheets`` で ``head`` タグ内に追加します。\
もうひとつは、\ ``javascript`` で、\ ``body`` 閉じタグの直前に追加します。\
これらの block で、サイトを通じて必要なすべてのスタイルシートや Javascript を収容することになります。

.. code-block:: html+jinja

    {# 'app/Resources/views/base.html.twig' #}
    <html>
        <head>
            {# ... #}

            {% block stylesheets %}
                <link href="{{ asset('/css/main.css') }}" type="text/css" rel="stylesheet" />
            {% endblock %}
        </head>
        <body>
            {# ... #}

            {% block javascripts %}
                <script src="{{ asset('/js/main.js') }}" type="text/javascript"></script>
            {% endblock %}
        </body>
    </html>

とても簡単ですね！\
では、子テンプレートでスタイルシートや Javascript を追加でインクルードしたいときはどうしましょうか。\
例えば、お問い合わせページがあって、\ *そのページでだけ*  ``contact.css`` を追加でインクルードしたいとしましょう。\
お問い合わせページでは次のようにします。

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Contact/contact.html.twig #}
    {# extends '::base.html.twig' #}

    {% block stylesheets %}
        {{ parent() }}

        <link href="{{ asset('/css/contact.css') }}" type="text/css" rel="stylesheet" />
    {% endblock %}

    {# ... #}

子テンプレートでは、単に ``stylesheets`` をオーバーライドし、新しいスタイルシートタグを置いてやります。\
とはいえ、親の block の内容に追加したい(、そして、\ *置き換え*\ たいわけではない)ので、\
Twig 関数の ``parent()`` を使って、ベーステンプレートの ``stylesheets`` 内のすべてをインクルードしてやるべきでしょう。

また、バンドルの ``Resources/public`` フォルダに配置したアセットをインクルードすることもできます。
この場合、\ ``php app/console assets:install target [--symlink]``
コマンドを実行して、アセットファイルを Web から参照できる正しい位置（デフォルトでは "web" ディレクトリ）へコピーまたはシンボリックリンクする必要があります。

.. code-block:: html+jinja

   <link href="{{ asset('bundles/acmedemo/css/contact.css') }}" type="text/css" rel="stylesheet" />

この結果、\ ``main.css`` と ``contact.css`` のスタイルシートの両方共をインクルードしたページとなります。

.. index::
   single: Templating; The templating service

\ ``templating`` サービスの設定と使用
-------------------------------------

Symfony2 におけるテンプレートシステムの心臓部は、テンプレート ``Engine`` です。\
テンプレートをレンダリングして、その内容を返す、特別なオブジェクトです。\
たとえば、コントローラ内でテンプレートを render するときは、\
実際には、テンプレートエンジンサービスを使用しているのです。

.. code-block:: php

    return $this->render('AcmeArticleBundle:Article:index.html.twig');

上記は、次のものと同等です。

.. code-block:: php

    $engine = $this->container->get('templating');
    $content = $engine->render('AcmeArticleBundle:Article:index.html.twig');

    return $response = new Response($content);

.. _template-configuration:

テンプレートエンジン(もしくは「サービス」)は、Symfony2 内部で予め自動的に設定済みです。\
もちろん、アプリケーションの設定ファイルで、さらなる設定が可能です。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating: { engines: ['twig'] }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:templating>
            <framework:engine id="twig" />
        </framework:templating>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'templating'      => array(
                'engines' => array('twig'),
            ),
        ));

設定オプションはいくつかあり、\ :doc:`Configuration Appendix</reference/configuration/framework>` で説明しています。

.. note::

   ``twig`` エンジンは、webprofiler(その他のサードパーティ製バンドルも)では必須となります。

.. index::
    single; Template; Overriding templates

.. _overriding-bundle-templates:

バンドルテンプレートの継承
--------------------------

Symfony2 コミュニティでは、たくさんの数の、そしてたくさんの機能を持ったバンドルを\
作成し、メンテしており、自慢できることです(`KnpBundles.com`_ 参照)。\
そのようなサードパーティ製のバンドルを使ったときに、そのテンプレートを、\
オーバライドしたりカスタマイズする必要があるかもしれません。

たとえば、\ ``AcmeBlogBundle`` というオープンソースなバンドルを、\
自分のプロジェクト(``src/Acme/BlogBundle`` にあるとします)にインクルードした場合を考えてみます。\
とても満足してはいるのだけれども、1点だけ、ブログの「リスト」ページのマークアップを、\
自分のプロジェクトに合うように変えたいとしましょう。\
``AcmeBlogBundle`` 内のコントローラ ``Blog`` をよく見てみると、次のようなコードが見つかりました。::

    public function indexAction()
    {
        $blogs = // some logic to retrieve the blogs

        $this->render('AcmeBlogBundle:Blog:index.html.twig', array('blogs' => $blogs));
    }

``AcmeBlogBundle:Blog:index.html.twig`` がレンダリングされる際、\
Symfony2 は、実は、2つの場所からテンプレートを探しています。

#. ``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig``

このバンドル内テンプレートをオーバーライドするには、単に、\ ``index.html.twig`` を\
``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig`` にコピーしてやります\
(ただし、\ ``app/Resources/AcmeBlogBundle`` ディレクトリは無いはずですので、作成する必要があります)。\
そして、そのテンプレートを自由にカスタマイズすればよいのです。

このロジックはバンドルのベーステンプレートにも当てはまります。\
``AcmeBlogBundle`` 内のテンプレートが、ベーステンプレート ``AcmeBlogBundle::layout.html.twig`` を継承している場合、\
先程の例と同様に、Symfony2 は次の2つの場所をさがします。

#. ``app/Resources/AcmeBlogBundle/views/layout.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/layout.html.twig``

オーバライドするには、前と同じように、\ ``app/Resources/AcmeBlogBundle/views/layout.html.twig`` にコピーします。\
そして、見た目が合うようにこれをカスタマイズしていきます。

俯瞰してみると、Symfony2 がテンプレートを探すときは、常に、まず ``app/Resources/{BUNDLE_NAME}/views/`` から探しているのがわかります。\
そこに何もなければ、続いてバンドルの ``Resources/views`` をチェックします。\
ですので、\ ``app/Resources`` 下に正しい構造でテンプレートを置いてやれば、すべてのバンドルテンプレートをオーバーライドすることができます。


.. _templating-overriding-core-templates:

.. index::
    single; Template; Overriding exception templates

コアテンプレートをオーバーライドする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 フレームワークこれ自体もバンドルですので、コアテンプレートも同様にオーバーライドできます。\
``TwigBundle`` は、内部に "exception" や "error" 用のテンプレートを多数持っていますが、\
これも、バンドルの ``Resources/views/Exception`` ディレクトリからコピーしてやればいいのです。\
どこにコピーするかは、もうおわかりですよね？\
``app/Resources/TwigBundle/views/Exception`` ディレクトリです。

.. index::
   single: Templating; Three-level inheritance pattern

3-level の継承
--------------

継承を使う際によくあるのが、3-level のアプローチです。\
このアプローチは、今まで見てきた次の3種のテンプレートからなります。

* ``app/Resources/views/base.html.twig`` を作成します。\
  これには、(先程の例のような)アプリケーションのメインレイアウトをいれます。\
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

このテンプレートは、セクションテンプレート(``AcmeBlogBundle::layout.html.twig``)を継承し、\
今度はセクションテンプレートが、アプリケーションレイアウト(``::base.html.twig``)を継承しています。\
これが、3-level 継承というものです。

アプリケーションを作るときは、たいてい、この 3-level 継承を使うか、もしくは、\
各ページテンプレートがアプリケーションテンプレートを直に継承する(たとえば ``{% extends '::base.html.twig' %}``)ことになるでしょう。\
3-level モデルは、ベンダーバンドルにより使用されるベストプラクティスで、\
バンドルのベーステンプレートが、アプリケーションのベースレイアウトを適切に継承するように、\
簡単にオーバーライドできます。

.. index::
   single: Templating; Output escaping

アウトプットエスケープ
----------------------

テンプレートから HTML を生成する際に、常にリスクになるのが、\
意図しない HTML や、危険なクライアントサイドコードを出力してしまうテンプレート変数の存在です。\
結果的に、動的なコンテンツが HTML を壊したり、悪意あるユーザが `Cross Site Scripting`_\(XSS)攻撃をするのを許してしまいます。\
その典型例を見ていきましょう。

.. configuration-block::

    .. code-block:: jinja

        Hello {{ name }}

    .. code-block:: html+php

        Hello <?php echo $name ?>

もしユーザーが次のような名前を入力したとしたらどうでしょうか。 ::

    <script>alert('hello!')</script>

アウトプットエスケープをしなければ、このテンプレートでは、\
Javascript のアラートボックスがポップアップしてしまうでしょう。 ::

    Hello <script>alert('hello!')</script>

上のスクリプトでは問題がないように見えますが、\
ここまでできてしまうことを知ったユーザに悪意があれば、\
何も知らない正当なユーザのセキュアな場所で悪さをするJavaScripを書くことがでできてしまうでしょう。

この問題への答えは、アウトプットエスケープをすることです。\
これが有効になっていれば、テンプレートは害を及ぼさない形でレンダリングされ、\
``script`` とそのまま文字通り画面に出力されます。::

    Hello &lt;script&gt;alert(&#39;helloe&#39;)&lt;/script&gt;

Twig と PHP では、異なるアプローチをとっています。\
Twig の場合であれば、アウトプットエスケープは常に on になっているので、安全です。\
PHP の場合は自動とはいかず、必要な場合には常に手動でエスケープする必要があります。

Twig の場合
~~~~~~~~~~~

Twig テンプレートを使っていれば、アウトプットエスケープはデフォルトで有効です。\
従って、ユーザがサブミットしたコードによる意図しない挙動を回避する、という意味ではそのままで安全です。\
デフォルトで、コンテンツのHTML 出力はエスケープされるべきとされているのです。

アウトプットエスケープを無効にして、信用に足る変数のレンダリングや、\
マークアップが含まれていてエスケープすべきでない変数をレンダリングする場合もあるでしょう。\
管理者がHTML コードを含む記事を書く場合を考えてみましょう。\
Twig では、デフォルトで記事をエスケープしてしまいます。\
通常は ``raw`` フィルターを追加して、レンダーしてやります。 ``{{ article.body | raw }}``

``{% block %}`` 単位、もしくはテンプレート全体でアウトプットエスケープを無効にすることもできます。\
詳細は、Twig ドキュメントの `Output Escaping`_ を参照してください。

PHP の場合
~~~~~~~~~~

PHP テンプレートを使用している場合は、アウトプットエスケープは自動的にはなりません。\
つまり、明示的に変数をエスケープするという選択をしなければ、安全でなくなります。\
エスケープを行うには、view のメソッドである ``escape()`` を使用します。::

    Hello <?php echo $view->escape($name) ?>

``escape()`` メソッドは、デフォルトでは、変数は HTML コンテクストでレンダリングする想定になっています\
(したがって、HTML 的に安全になるように変数はエスケープされます)。\
2つ目の引数でコンテキストを変更できます。\
例えば、Javascript 内で何か出力したいときは、\ ``js`` コンテキストを使用します。

.. code-block:: js

    var myMsg = 'Hello <?php echo $view->escape($name, 'js') ?>';

.. index::
   single: Templating; Formats

.. _template-formats:

テンプレートフォーマット
------------------------

テンプレートは、\ *あらゆる*\ フォーマットでコンテンツをレンダーするための方法です。\
ほとんどはHTML コンテンツをレンダリングするのにテンプレートを使うことになるでしょうが、\
Javascript や CSS、XML、その他考えうるフォーマットでも簡単に生成することもできます。

たとえば、同一の「リソース」でも、複数のフォーマットでレンダリングされることはよくあります。\
index ページを XML でレンダリングしたいときは、テンプレート名にフォーマットを含ませてやります。

* *XML テンプレート名*: ``AcmeArticleBundle:Article:index.xml.twig``
* *XML テンプレートファイル名*: ``index.xml.twig``

ただし、これは命名規則以外の何者でもなく、\
フォーマットに基づいたテンプレートレンダリングがされるわけではありません。

1つのコントローラで、「リクエストフォーマット」に応じて複数のフォーマットでレンダリングしたい場合も多くあると思います。\
次のようにするのが一般的でしょう。

.. code-block:: php

    public function indexAction()
    {
        $format = $this->getRequest()->getRequestFormat();

        return $this->render('AcmeBlogBundle:Blog:index.'.$format.'.twig');
    }

``Request`` オブジェクトの ``getRequestFormat`` は、デフォルトでは ``html`` をかえしますが、\
ユーザによりリクエストされたフォーマットに基づき、どの様なフォーマットでも返すことができます。\
リクエストフォーマットは、ほとんどの場合、ルーティングによって扱われます。\
たとえば、\ ``/contact`` であれば ``html``\ 、\ ``contact.xml``\ であれば ``xml`` というふうな設定ができます。\
詳細は、\ :ref:`ルーティング <advanced-routing-example>`\ を参照してください。

フォーマットをリンクに入れたい場合は、パラメータに ``_format`` キーで指定してください。

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('article_show', {'id': 123, '_format': 'pdf'}) }}">
            PDF Version
        </a>

    .. code-block:: html+php

        <a href="<?php echo $view['router']->generate('article_show', array('id' => 123, '_format' => 'pdf')) ?>">
            PDF Version
        </a>

Final Thoughts
--------------

Symfony のテンプレートエンジンは強力なツールで、\
表層的なコンテンツを、HTML や、XMLその他フォーマットで生成したい場合に使うツールです。\
テンプレートは、コントローラ内でコンテンツを生成する一般的な方法ではありますが、\
特に必須というわけではありません。\
コントローラによって返されるべき ``Response`` オブジェクトは、テンプレートを使用しなくても作成可能です。

.. code-block:: php

    // レンダリングされたテンプレートをコンテンツとする Response オブジェクト
    $response = $this->render('AcmeArticleBundle:Article:index.html.twig');

    // 単純なテキストをコンテンツとする Response オブジェクト
    $response = new Response('response content');

Symfony のテンプレートエンジンはとても柔軟で、デフォルトでは2種類のレンダラが利用可能です。\
従来の *PHP* テンプレートと、洒落ていて強力な *Twig* です。\
両者とも、階層的なテンプレートをサポートしており、\
一般的なタスクのほとんどをこなすことのできるヘルパ関数が豊富にパッケージされています。

全体として、テンプレートの主題は、自由に使える強力なツールであると考えられるべきです。\
テンプレートをレンダーする必要のない場合もあるかもしれませんが、\
Symfony2 では、その場合でも問題ありません。

Cookbook でもっと学ぶ
---------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/error_pages`

.. _`Twig`: http://twig.sensiolabs.org
.. _`KnpBundles.com`: http://knpbundles.com
.. _`Cross Site Scripting`: http://en.wikipedia.org/wiki/Cross-site_scripting
.. _`Output Escaping`: http://twig.sensiolabs.org/doc/api.html#escaper-extension
.. _`タグ`: http://twig.sensiolabs.org/doc/tags/index.html
.. _`フィルタ`: http://twig.sensiolabs.org/doc/filters/index.html
.. _`エクステンションを追加する`: http://twig.sensiolabs.org/doc/extensions.html

.. 2011/08/08 gilbite d7f118ff2c3f5fb73f1d2be27d2c88f166fbc10d
.. 2011/12/28 hidenorigoto 5034f36ebb0abe7aa86bb8d90f3a3454fb28e8b2
