.. When Flat PHP meets Symfony
   ===========================

フラットな PHP が Symfony に出会う時
====================================

.. **Why is Symfony2 better than just opening up a file and writing flat PHP?**

**なぜ Symfony2 は単にファイルを開いてフラットな PHP を書くよりも良いのでしょうか？**

.. If you've never used a PHP framework, aren't familiar with the MVC philosophy,
   or just wonder what all the *hype* is around Symfony2, this chapter is for
   you. Instead of *telling* you that Symfony2 allows you to develop faster and
   better software than with flat PHP, you'll see for yourself.

もしあなたが PHP フレームワークを使ったことがなかったり、 MVC の哲学に親しみが
なかったり、あるいは Symfony2 周辺の *宣伝文句* が何なのか気になっただけ
だったりしたら、この章はあなたのためのものです。 Symfony2 がフラットな PHP を
使うよりも素早くよりよいソフトウェアを開発できるようにすることを *教える*
代わりに、あなた自身でそれが理解できるでしょう。

.. In this chapter, you'll write a simple application in flat PHP, and then
   refactor it to be more organized. You'll travel through time, seeing the
   decisions behind why web development has evolved over the past several years
   to where it is now. 

この章では、まずシンプルなアプリケーションをフラットな PHP で書きます。
それから、より体系づけられたものへ書き直していきます。タイムトラベルを
しながら、なぜ Web 開発が過去数年間にわたり現在の姿に発展してきたのかの
背景にある決断を見ていきます。

.. By the end, you'll see how Symfony2 can rescue you from mundane tasks and
   let you take back control of your code.

最後には、 Symfony2 がどのように一般的なタスクからあなたを解放し、自身の
コードのコントロールを取り戻すようお手伝いするかを知ることになります。

.. A simple Blog in flat PHP
   -------------------------

フラットな PHP による単純なブログ
---------------------------------

.. In this chapter, you'll build the token blog application using only flat PHP.
   To begin, create a single page that displays blog entries that have been
   persisted to the database. Writing in flat PHP is quick and dirty:

この章では、フラットな PHP だけを使った外形だけのブログアプリケーションを
作ります。始めに、データベースに永続化されたブログのエントリを表示する一つの
ページを作ってください。フラットな PHP を書くのは素早いけれど乱暴です。

.. code-block:: html+php

    <?php

    // index.php

    $link = mysql_connect('localhost', 'myuser', 'mypassword');
    mysql_select_db('blog_db', $link);

    $result = mysql_query('SELECT id, title FROM post', $link);

    ?>

    <html>
        <head>
            <title>投稿の一覧</title>
        </head>
        <body>
            <h1>投稿の一覧</h1>
            <ul>
                <?php while ($row = mysql_fetch_assoc($result)): ?>
                <li>
                    <a href="/show.php?id=<?php echo $row['id'] ?>">
                        <?php echo $row['title'] ?>
                    </a>
                </li>
                <?php endwhile; ?>
            </ul>
        </body>
    </html>

    <?php
    mysql_close($link);

.. That's quick to write, fast to execute, and, as your app grows, impossible
   to maintain. There are several problems that need to be addressed:

素早く書けて、高速に実行できますが、アプリケーションが大きくなるにつれて、
メンテナンスが手に負えなくなります。解決すべきいくつかの問題があります。

.. * **No error-checking** What if the connection to the database fails?

.. * **Poor organization**: If the application grows, this single file will become
     increasingly unmaintainable. Where should you put code to handle a form
     submission? How can you validate data? Where should code go for sending
     emails?

.. * **Difficult to reuse code**: Since everything is in one file, there's no
     way to reuse any part of the application for other "pages" of the blog.

* **エラーチェックがありません** データベースへの接続が失敗した場合はどうなるのでしょう？

* **体系化されていません** アプリケーションが複雑になっていくと、この
  1つのファイルはどんどんメンテナンスできなくなっていきます。フォームの
  送信を扱うコードはどこに入れたらよいでしょうか？ Eメールを送信する
  コードはどうしたらいいでしょうか？

* **コードの再利用が難しいです** 全てが1つのファイルにまとまっているので、
  アプリケーションの他の「ページ」のために一部を再利用する方法がありません。

.. note::

    ここで述べられていない他の問題として、データベースが MySQL に固定されて
    しまうということがあります。ここではこの話に触れませんが、 Symfony2 は、
    `Doctrine`_ というデータベースの抽象化とマッピングを行うライブラリと
    完全に統合されています。

..    Another problem not mentioned here is the fact that the database is
      tied to MySQL. Though not covered here, Symfony2 fully integrates `Doctrine`_,
      a library dedicated to database abstraction and mapping.

.. Let's get to work on solving these problems and more.

さあ、これらの問題を解決していきましょう。

.. Isolating the Presentation
   ~~~~~~~~~~~~~~~~~~~~~~~~~~

表示部分の分離
~~~~~~~~~~~~~~

.. The code can immediately gain from separating the application "logic" from
   the code that prepares the HTML "presentation":

このコードは、HTML 表現を準備するコードから、アプリケーションの
「ロジック」を分離することで、すぐに改善できます。

.. code-block:: html+php

    <?php

    // index.php

    $link = mysql_connect('localhost', 'myuser', 'mypassword');
    mysql_select_db('blog_db', $link);

    $result = mysql_query('SELECT id, title FROM post', $link);

    $posts = array();
    while ($row = mysql_fetch_assoc($result)) {
        $posts[] = $row;
    }

    mysql_close($link);

    // include the HTML presentation code
    require 'templates/list.php';

.. The HTML code is now stored in a separate file (``templates/list.php``), which
   is primarily an HTML file that uses a template-like PHP syntax:

HTML コードは別なファイル (``templates/list.php``) に保存されるようになりました。
これは本来、テンプレート風の PHP 文法を使う HTML ファイルです。

.. code-block:: html+php

    <html>
        <head>
            <title>投稿のリスト</title>
        </head>
        <body>
            <h1>投稿のリスト</h1>
            <ul>
                <?php foreach ($posts as $post): ?>
                <li>
                    <a href="/read?id=<?php echo $post['id'] ?>">
                        <?php echo $post['title'] ?>
                    </a>
                </li>
                <?php endforeach; ?>
            </ul>
        </body>
    </html>

.. By convention, the file that contains all of the application logic - ``index.php`` -
   is known as a "controller". The term :term:`controller` is a word you'll hear
   a lot, regardless of the language or framework you use. It refers simply
   to the area of *your* code that processes user input and prepares the response.

慣例によって、全てのアプリケーションのロジックを含むファイル「 ``index.php`` 」は
「コントローラ」と呼ばれます。 :term:`コントローラ` という用語は、あなたの Web アプリケーションの
ためにどんな言語やフレームワークを選んだかに関係なく、よく聞くことでしょう。
これはシンプルに、リクエストからの入力を受け取り、レスポンスを返す *あなたの* コードの
一部分を指しています。

.. In this case, our controller prepares data from the database and then includes
   a template to present that data. With the controller isolated, you could
   easily change *just* the template file if you needed to render the blog
   entries in some other format (e.g. ``list.json.php`` for JSON format). 

この場合、コントローラはデータベースからのデータを準備し、それからそのデータを提供する
テンプレートをインクルードします。コントローラが分離することによって、
何か他のフォーマット (例えば JSON フォーマットの ``list.json.php``) で
ブログのエントリをレンダリングする必要があった場合に、テンプレートファイル *だけ*
を簡単に変更することができます。

.. Isolating the Application (Domain) Logic
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

アプリケーション (ドメイン) ロジックの分離
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. So far the application contains only one page. But what if a second page
   needed to use the same database connection, or even the same array of blog
   posts? Refactor the code so that the core behavior and data-access functions
   of the application are isolated in a new file called ``model.php``:

今のところアプリケーションは1つのページしか含んでいませんが、2番目の
ページが同じデータベース接続、あるいは同じ投稿の配列を使用する必要がある場合は
どうでしょうか？ アプリケーションのコアの動作とデータアクセスの機能が
``mode.php`` という新しいファイルに分離されるよう、コードをリファクタリング
してみましょう。

.. code-block:: html+php

    <?php

    // model.php

    function open_database_connection()
    {
        $link = mysql_connect('localhost', 'myuser', 'mypassword');
        mysql_select_db('blog_db', $link);

        return $link;
    }

    function close_database_connection($link)
    {
        mysql_close($link);
    }

    function get_all_posts()
    {
        $link = open_database_connection();

        $result = mysql_query('SELECT id, title FROM post', $link);
        $posts = array();
        while ($row = mysql_fetch_assoc($result)) {
            $posts[] = $row;
        }

        close_database_connection($link);

        return $posts;
    }

.. tip::

    ``model.php`` というファイル名が使われているのは、アプリケーションの
    ロジックとデータアクセスが伝統的に「モデル」というレイヤーだからです。
    うまく体系付けられたアプリケーションでは、「ビジネスロジック」を表す
    コードの大部分は、モデル内に存在するべきです (コントローラに存在するの
    とは対照的に) 。そしてこの例とは違って、モデルの一部分のみが実際に
    データベースへのアクセスに関わることになります。

..   The filename ``model.php`` is used because the logic and data access of
     an application is traditionally known as the "model" layer. In a well-organized
     application, the majority of the code representing your "business logic"
     should live in the model (as opposed to living in a controller). And unlike
     in this example, only a portion (or none) of the model is actually concerned
     with accessing a database.

.. The controller (``index.php``) is now very simple:

コントローラ (``index.php``) はとてもシンプルになります。

.. code-block:: html+php

    <?php

    require_once 'model.php';

    $posts = get_all_posts();

    require 'templates/list.php';

.. Now, the sole task of the controller is to get data from the model layer of
   the application (the model) and to call a template to render that data.
   This is a very simple example of the model-view-controller pattern.

この時点で、コントローラの唯一のタスクは、アプリケーションのモデルレイヤー
(モデル) からデータを取り出し、そのデータをレンダリングするためにテンプレートを
呼び出すことです。これは、モデル-ビュー-コントローラパターンのとても単純な
例です。

.. Isolating the Layout
   ~~~~~~~~~~~~~~~~~~~~

レイアウトの分離
~~~~~~~~~~~~~~~~

.. At this point, the application has been refactored into three distinct pieces
   offering various advantages and the opportunity to reuse almost everything
   on different pages.

この時点でアプリケーションは、いくつかの有利な点を持つ3つの明確な部品に
リファクタリングされ、別のページでほとんど全てを再利用できる機会を得ます。

.. The only part of the code that *can't* be reused is the page layout.
   Fix that by creating a new ``layout.php`` file:

コードの中で再利用 *できない* 唯一の部分は、ページレイアウトです。 ``layout.php``
ファイルを新しく作成して、これを修正しましょう。

.. code-block:: html+php

    <!-- templates/layout.php -->
    <html>
        <head>
            <title><?php echo $title ?></title>
        </head>
        <body>
            <?php echo $content ?>
        </body>
    </html>

.. The template (``templates/list.php``) can now be simplified to "extend"
   the layout:

レイアウトを「拡張」するようテンプレート (``templates/list.php``) を
単純化できました。

.. code-block:: html+php

    <?php $title = '投稿のリスト' ?>

    <?php ob_start() ?>
        <h1>投稿のリスト</h1>
        <ul>
            <?php foreach ($posts as $post): ?>
            <li>
                <a href="/read?id=<?php echo $post['id'] ?>">
                    <?php echo $post['title'] ?>
                </a>
            </li>
            <?php endforeach; ?>
        </ul>
    <?php $content = ob_get_clean() ?>

    <?php include 'layout.php' ?>

.. We've now introduced a methodology that that allows for the reuse of the
   layout. Unfortunately, you'll also notice that we've had to use a few ugly
   PHP functions (``ob_start()``, ``ob_end_clean()``) in the template. Symfony2
   uses a ``Templating`` component that allows this to be accomplished cleanly
   and easily. You'll see it in action shortly.

ここで、レイアウトの再利用を可能にする方法を披露します。残念なことに、
これを可能にするために、いくつかの格好悪い PHP の関数 (``ob_start()`` と ``ob_end_clean()``)
をテンプレート内で使わなければならないことにお気づきだと思います。
Symfony2 はクリーンで簡単にこれを実現できる ``Templating`` コンポーネントを使います。
これはもうすぐ実践の中で見ていくことになります。

.. Adding a Blog "show" Page
   -------------------------

ブログの「show (単独表示) 」ページを追加
----------------------------------------

.. The blog "list" page has now been refactored so that the code is better-organized
   and reusable. To prove it, add a blog "show" page, which displays an
   individual blog post identified by an ``id`` query parameter.

ブログの「list (一覧表示)」ページは、より体系付けられて再利用可能なコードに
なるようリファクタリングされました。これを証明するために、 ``id`` をクエリー
パラメータとしてそれぞれのブログの投稿を表示する、「show (単独表示)」ページを
追加しましょう。

.. To begin, we'll need a new function in the ``model.php`` file that retrieves
   an individual blog result based on a given id::

まず始めに、与えられた ID を元にそれぞれのブログの結果を取得する関数を
``model.php`` ファイルに追加する必要があります::

    // model.php
    function get_post_by_id($id)
    {
        $link = open_database_connection();

        $id = mysql_real_escape_string($id);
        $query = 'SELECT date, title, body FROM post WHERE id = '.$id;
        $result = mysql_query($query);
        $row = mysql_fetch_assoc($result);

        close_database_connection($link);

        return $row;
    }

.. Next, create a new file called ``show.php`` - our controller for this new
   page:

次に、この新しいページのためのコントローラである ``show.php`` という
新しいファイルを作ってください。

.. code-block:: html+php

    <?php

    require_once 'model.php';

    $post = get_post_by_id($_GET['id']);

    require 'templates/show.php';

.. Finally, create the new template file - ``templates/show.php`` - to render
   the individual blog:

最後に、それぞれの投稿を表示するための ``templates/show.php`` という新しい
テンプレートファイルを作ってください。

.. code-block:: html+php

    <?php $title = $post['title'] ?>

    <?php ob_start() ?>
        <h1><?php echo $post['title'] ?></h1>

        <div class="date"><?php echo $post['date'] ?></div>
        <div class="body">
            <?php echo $post['body'] ?>
        </div>
    <?php $content = ob_get_clean() ?>

    <?php include 'layout.php' ?>

.. Creating the second page is now very easy and no code is duplicated. Still,
   this page introduces even more lingering problems that a framework can solve
   for you. For example, a missing or invalid ``id`` query parameter will cause
   the page to crash. It would be better if this caused a 404 page to be rendered,
   but this can't really be done easily yet. Worse, had you forgotten to clean
   the ``id`` parameter via the ``mysql_real_escape_string()`` function, your
   entire database would be at risk for an SQL injection attack.

2番目のページを作るのは、とても簡単で、重複したコードもありません。まだ
このページには、フレームワークが解決できるさらにやっかいな問題があります。
例えば、「id」クエリーパラメータが存在しなかったり不正な場合、ページが
クラッシュする原因になります。このような問題では 404 ページを表示する方がよい
ですが、まだこれは簡単には実現できません。さらに問題なことに、
``mysql_real_escape_string()`` 関数を経由して ``id`` パラメータをクリーンに
し忘れると、データベース全体が SQL インジェクション攻撃のリスクにさらされる
ことになります。

.. Another major problem is that each individual controller file must include
   the ``model.php`` file. What if each controller file suddenly needed to include
   an additional file or perform some other global task (e.g. enforce security)?
   As it stands now, that code would need to be added to every controller file.
   If you forget to include something in one file, hopefully it doesn't relate
   to security...

それ以外の大きな問題として、それぞれのコントローラのファイルが ``model.php``
ファイルを含まなくてはならないということです。それぞれのコントローラファイルが、
突然追加のファイルを読み込む必要に迫られたり、その他のグローバルなタスク
(例えばセキュリティの向上など) を実行する必要が出た場合、どうなるでしょう。
現状では、それを実現するためのコードは全てのコントローラのファイルに追加する
必要があります。もし何かをあるファイルに含むのを忘れてしまった時、それが
セキュリティに関係ないといいのですが…。

.. A "Front Controller" to the Rescue
   ----------------------------------

「フロントコントローラ」の出番
------------------------------

.. The solution is to use a front controller: a single PHP file through which
   *all* requests are processed. With a front controller, the URIs for the
   application change slightly, but start to become more flexible::

解決策は、フロントコントローラを使うことです。これは、 *全ての* リクエストが
処理される際に通過する一つの PHP ファイルです。フロントコントローラによって、
アプリケーションの URI は少し変更されますが、より柔軟になり始めます。

.. code-block:: text

    フロントコントローラなしの場合
    /index.php          => ブログ一覧表示ページ (index.php が実行されます)
    /show.php           => ブログ単独表示ページ (show.php が実行されます)

    index.php をフロントコントローラとして使用した場合
    /index.php          => ブログ一覧表示ページ (index.php が実行されます)
    /index.php/show     => ブログ単独表示ページ (index.php が実行されます)

..    Without a front controller
    /index.php          => Blog list page (index.php executed)
    /show.php           => Blog show page (show.php executed)

..    With index.php as the front controller
    /index.php          => Blog list page (index.php executed)
    /index.php/show     => Blog show page (index.php executed)

.. tip::
    URI の ``index.php`` という一部分は、 Apache のリライトルール
    (あるいはそれと同等の仕組み) を使っている場合は、省略することが
    できます。この場合、ブログの単独表次ページの URI は、単純に
    ``/show`` になります。

..    The ``index.php`` portion of the URI can be removed if using Apache
      rewrite rules (or equivalent). In that case, the resulting URI of the
      blog show page would simply be ``/show``.

.. When using a front controller, a single PHP file (``index.php`` in this case)
   renders *every* request. For the blog show page, ``/index.php/show`` will
   actually execute the ``index.php`` file, which is now responsible for routing
   requests internally based on the full URI. As you'll see, a front controller
   is a very powerful tool.

フロントコントローラを使用する時は、一つの PHP ファイル (今回は ``index.php``) が
*全ての* リクエストをレンダリングします。ブログの単一表示ページでは、
``/index.php/show`` という URI で実際には、完全な URI に基づいてルーティングの
リクエストに内部的に応える ``index.php`` ファイルが実行されます。ここで見たように、
フロントコントローラはとてもパワフルなツールなのです。

.. Creating the Front Controller
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

フロントコントローラの作成
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. We're about to take a **big** step with our application. With one file handling
   all requests, we can centralize things such as security handling, configuration
   loading, and routing. In our application, ``index.php`` must now be smart
   enough to render the blog list page *or* the blog show page based on the
   requested URI:

我々のアプリケーションに関して、 **大きな** 一歩を踏み出そうとしています。
全てのリクエストを扱う一つのファイルによって、セキュリティの扱いや、設定の
読み込み、ルーティングといったことを集中的に扱えるようになります。我々の
アプリケーションでは ``index.php`` が、リクエストされた URI に基づいて、
ブログの一覧表示ページ *あるいは* 単一表示ページをレンダリングするのに
十分なぐらい洗練されている必要があります。

.. code-block:: html+php

    <?php

    // index.php

    // グローバルライブラリの読み込みと初期化
    require_once 'model.php';
    require_once 'controllers.php';

    // リクエストを内部的にルーティング
    $uri = $_REQUEST['REQUEST_URI'];
    if ($uri == '/index.php') {
        list_action();
    } elseif ($uri == '/index.php/show' && isset($_GET['id'])) {
        show_action($_GET['id']);
    } else {
        header('Status: 404 Not Found');
        echo '<html><body><h1>ページが見つかりません</h1></body></html>';
    }

.. For organization, both controllers (formerly index.php and show.php)
   are now PHP functions and each has been moved into a separate file,
   controllers.php:

コードの体系化のために、2つのコントローラ (以前の index.php と show.php)
は、 PHP の関数になり、それぞれは別のファイル controllers.php に
移動されました。

.. code-block:: php

    function list_action()
    {
        $posts = get_all_posts();
        require 'templates/list.php';
    }

    function show_action($id)
    {
        $post = get_post_by_id($id);
        require 'templates/show.php';
    }

.. As a front controller, ``index.php`` has taken on an entirely new role, one
   that includes loading the core libraries and routing the application so that
   one of the two controllers (the ``list_action()`` and ``show_action()``
   functions) is called. In reality, the front controller is beginning to look and
   act a lot like Symfony2's mechanism for handling and routing requests.

フロントコントローラとして、 ``index.php`` は全く新しい役割を引き受ける
ことになりました。それは、コアライブラリを読み込み、2つのコントローラ
(``list_action()`` と ``show_action()`` 関数) のうちの1つを呼び出せるように
アプリケーションをルーティングすることです。
実際にこのフロントコントローラは、リクエストを取り扱いルーティングする Symfony2 の
メカニズムによく似た見た目と動作をし始めています。

.. tip::

   フロントコントローラのもう一つの利点が、柔軟性のある URL です。
   コードのたった1箇所だけを変更すれば、ブログ単一表示ページの URL を
   ``/show`` から ``/read`` に変更できることに注目してください。
   以前は、ファイル全体の名前を変更する必要がありましたね。 Symfony2 では、
   URL の取り扱いはもっとずっと柔軟性があります。

..   Another advantage of a front controller is flexible URLs. Notice that
   the URL to the blog show page could be changed from ``/show`` to ``/read``
   by changing code in only one location. Before, an entire file needed to
   be renamed. In Symfony2, URLs are even more flexible.

.. By now, the application has evolved from a single PHP file into a structure
   that is organized and allows for code reuse. You should be happier, but far
   from satisfied. For example, the "routing" system is fickle, and wouldn't
   recognize that the list page (/index.php) should be accessible also via /
   (if Apache rewrite rules were added). Also, instead of developing the blog,
   a lot of time is being spent working on the "architecture" of the code
   (e.g. routing, calling controllers, templates, etc.). More time will need
   to be spent to handle form submissions, input validation, logging and
   security. Why should you have to reinvent solutions to all these routine
   problems?

ここまで、アプリケーションを単一の PHP ファイルから、体系化されてコードの
再利用ができる構造へと発展させてきました。これでハッピーになるべきですが、
満足からは程遠いでしょう。例えば、「ルーティング」システムは気まぐれで、
一覧表示ページ (/index.php) が / (Apacheのリライトルールが追加されている場合)
からでもアクセス可能であるべきだということを認識できません。また、ブログを
開発する代わりに、コードの「アーキテクチャ」 (例えばルーティングや呼び出す
コントローラ、テンプレートなど) にたくさんの時間を費やしています。より多くの
時間を、フォームの送信の扱い、入力のバリデーション、ロギングやセキュリティ
といったことに費やす必要があるでしょう。なぜこれら全てのありふれた問題への
解決策を再発明しなければならないのでしょうか？

.. Add a Touch of Symfony2
   ~~~~~~~~~~~~~~~~~~~~~~~

ちょっと Symfony2 の考えを加える
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. Symfony2 to the rescue. Before actually using Symfony2, you need to make
   sure PHP knows how to find the Symfony2 classes. This is accomplished via
   an autoloader that Symfony provides. An autoloader is a tool that makes it
   possible to start using PHP classes without explicitly including the file
   containing the class.

Symfony2 の出番です。実際に Symfony2 を使う前に、 Symfony2 のクラスを
どのように見つけるのかを PHP が知っているようにする必要があります。
これは、 Symfony2 が提供するオートローダーを通じて実現されます。
オートローダーは、クラスを含むファイルを明確に含まなくても、 PHP のクラスを
使い始められるようにするツールです。

.. First, `download symfony`_ and place it into a ``vendor/symfony/`` directory.
   Next, create an ``app/bootstrap.php`` file. Use it to ``require`` the two
   files in the application and to configure the autoloader:

まず最初に、 `symfony をダウンロード`_ し、 ``vendor/symfony`` ディレクトリに
配置してください。次に、 ``app/bootstrap.php`` ファイルを作ってください。
アプリケーション内の2つのファイルを ``要求`` し、オートローダーを設定するために
このファイルを使います。

.. code-block:: html+php

    <?php
    // bootstrap.php
    require_once 'model.php';
    require_once 'controllers.php';
    require_once 'vendor/symfony/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';

    $loader = new Symfony\Component\ClassLoader\UniversalClassLoader();
    $loader->registerNamespaces(array(
        'Symfony' => __DIR__.'/vendor/symfony/src',
    ));

    $loader->register();

.. This tells the autoloader where the ``Symfony`` classes are. With this, you
   can start using Symfony classes without using the ``require`` statement for
   the files that contain them.

このファイルは、オートローダーに ``Symfony`` クラスがどこにあるかを知らせます。
これにより、 Symfony クラスを含むファイルで ``require`` ステートメントを
使わずに、 Symfony クラスを使い始めることができます。

.. Core to Symfony's philosophy is the idea that an application's main job is
   to interpret each request and return a response. To this end, Symfony2 provides
   both a :class:`Symfony\\Component\\HttpFoundation\\Request` and a
   :class:`Symfony\\Component\\HttpFoundation\\Response` class. These classes are
   object-oriented representations of the raw HTTP request being processed and
   the HTTP response being returned. Use them to improve the blog:

Symfony の哲学の核は、アプリケーションの主なジョブはそれぞれのリクエストを
解釈し、レスポンスを返すことであるという考え方です。この目的のために、
Symfony2 は :class:`Symfony\\Component\\HttpFoundation\\Request` と
:class:`Symfony\\Component\\HttpFoundation\\Response` という2つのクラスを
提供しています。これらのクラスは、処理されるべき生の HTTP リクエストと、
返される HTTP レスポンスのオブジェクト指向での実装になっています。ブログを
改善するために、これらを使いましょう。

.. code-block:: html+php

    <?php
    // index.php
    require_once 'app/bootstrap.php';

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpFoundation\Response;

    $request = Request::createFromGlobals();

    $uri = $request->getPathInfo();
    if ($uri == '/') {
        $response = list_action();
    } elseif ($uri == '/show' && $request->query->has('id')) {
        $response = show_action($request->query->get('id'));
    } else {
        $html = '<html><body><h1>Page Not Found</h1></body></html>';
        $response = new Response($html, 404);
    }

    // ヘッダーを返し、レスポンスを送る
    $response->send();

.. The controllers are now responsible for returning a ``Response`` object.
   To make this easier, you can add a new ``render_template()`` function, which,
   incidentally, acts quite a bit like the Symfony2 templating engine:

コントローラは、 ``Response`` オブジェクトを返す責任を持つように
なりました。これを簡単にするために、新しく ``render_template()`` 関数を
追加できます。ちなみに、この関数は Symfony2 のテンプレートエンジンとちょっと
似た動きをします。

.. code-block:: php

    // controllers.php
    use Symfony\Component\HttpFoundation\Response;

    function list_action()
    {
        $posts = get_all_posts();
        $html = render_template('templates/list.php');

        return new Response($html);
    }

    function show_action($id)
    {
        $post = get_post_by_id($id);
        $html = render_template('templates/show.php');

        return new Response($html);
    }

    // テンプレートをレンダリングするためのヘルパー関数
    function render_template($path)
    {
        ob_start();
        require $path;
        $html = ob_end_clean();

        return $html;
    }

.. By bringing in a small part of Symfony2, the application is more flexible and
   reliable. The ``Request`` provides a dependable way to access information
   about the HTTP request. Specifically, the ``getPathInfo()`` method returns
   a cleaned URI (always returning ``/show`` and never ``/index.php/show``).
   So, even if the user goes to ``/index.php/show``, the application is intelligent
   enough to route the request through ``show_action()``.

Symfony2 の一部分を使うことによって、アプリケーションはより柔軟で
信頼できるものになりました。 ``Request`` は HTTP リクエストに関する情報に
アクセスするための信頼できる仕組みを提供します。具体的にいうと、
``getPathInfo()`` メソッドは整理された URI (常に ``/show`` で、
``/index.php/show`` ではない) を返します。そのため、もしユーザが ``/index.php/show``
にアクセスしたとしても、アプリケーションは ``show_action()`` によって
リクエストをルーティングするインテリジェントさを持っています。

.. The ``Response`` object gives flexibility when constructing the HTTP response,
   allowing HTTP headers and content to be added via an object-oriented interface.
   And while the responses in this application are simple, this flexibility
   will pay dividends as your application grows.

``Response`` オブジェクトは、 HTTP ヘッダーとコンテンツをオブジェクト指向の
インタフェースを介して追加できるようにすることで、HTTP レスポンスを構成する際に
柔軟性を提供しています。そして、アプリケーションのレスポンスがシンプルな
ために、この柔軟性はアプリケーションが成長するのに大きな利点があるのです。

.. The Sample Application in Symfony2
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2でのサンプルアプリケーション
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. The blog has come a *long* way, but it still contains a lot of code for such
   a simple application. Along the way, we've also invented a simple routing
   system and a method using ``ob_start()`` and ``ob_end_clean()`` to render
   templates. If, for some reason, you needed to continue building this "framework"
   from scratch, you could at least use Symfony's standalone `Routing`_ and
   `Templating`_ components, which already solve these problems.

ブログは *大きな* 成長をしてきました。しかし、まだこの程度の小さなアプリケーション
なのにたくさんのコードを含んでいます。ここに至るまで、単純なルーティング
システムや、テンプレートをレンダリングするため ``ob_start()`` と ``ob_end_clean()``
を使ったメソッドを開発してきました。もし、何らかの理由で、この「フレームワーク」を
作り続ける必要があるのなら、これらの問題を既に解決している Symfony のスタンドアローンの
`Routing`_ と `Templating`_ コンポーネントを最低でも使うことができたでしょう。

.. Instead of re-solving common problems, you can let Symfony2 take care of
   them for you. Here's the same sample application, now built in Symfony2:

一般的な問題を改めて解決する代わりに、 Symfony2 にそれらの面倒を見させる
ことができます。以下が Symfony2 を使った同じサンプルアプリケーションです。

.. code-block:: html+php

    <?php
    // src/Acme/BlogBundle/Controller/BlogController.php

    namespace Acme\BlogBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class BlogController extends Controller
    {
        public function listAction()
        {
            $blogs = $this->container->get('doctrine.orm.entity_manager')
                ->createQuery('SELECT b FROM AcmeBlog:Blog b')
                ->execute();

            return $this->render('AcmeBlogBundle:Blog:list.html.php', array('blogs' => $blogs));
        }

        public function showAction($id)
        {
            $blog = $this->container->get('doctrine.orm.entity_manager')
                ->createQuery('SELECT b FROM AcmeBlog:Blog b WHERE id = :id')
                ->setParameter('id', $id)
                ->getSingleResult();

            return $this->render('AcmeBlogBundle:Blog:show.html.php', array('blog' => $blog));
        }
    }

.. The two controllers are still lightweight. Each uses the Doctrine ORM library
   to retrieve objects from the database and the ``Templating`` component to
   render a template and return a ``Response`` object. The list template is
   now quite a bit simpler:

2つのコントローラはまだ軽量です。それぞれ、データベースからオブジェクトを
取り出すために Doctrine ORM ライブラリを使用し、テンプレートをレンダリングして
``Response`` オブジェクトを返すために ``Templating`` コンポーネントを
使用しています。一覧表示のテンプレートは少しシンプルになりました。

.. code-block:: html+php

    <!-- src/Acme/BlogBundle/Resources/views/Blog/list.html.php --> 
    <?php $view->extend('::layout.html.php') ?>

    <?php $view['slots']->set('title', '投稿のリスト') ?>

    <h1>投稿のリスト</h1>
    <ul>
        <?php foreach ($posts as $post): ?>
        <li>
            <a href="<?php echo $view['router']->generate('blog_show', array('id' => $post->getId())) ?>">
                <?php echo $post->getTitle() ?>
            </a>
        </li>
        <?php endforeach; ?>
    </ul>

.. The layout is nearly identical:

レイアウトはほとんど全く同じです。

.. code-block:: html+php

    <!-- app/Resources/views/layout.html.php -->
    <html>
        <head>
            <title><?php echo $view['slots']->output('title', 'デフォルトのタイトル') ?></title>
        </head>
        <body>
            <?php echo $view['slots']->output('_content') ?>
        </body>
    </html>

.. note::

    単一表示のテンプレートはエクササイズとして残しておきます。一覧表示の
    テンプレートを元にして作成するのは簡単なはずです。

..    We'll leave the show template as an exercise, as it should be trivial to
    create based on the list template.

.. When Symfony2's engine (called the ``Kernel``) boots up, it needs a map so
   that it knows which controllers to execute based on the request information.
   A routing configuration map provides this information in a readable format::

Symfony2 のエンジン (``カーネル`` と呼ばれます) が起動する時には、
リクエスト情報を元にどのコントローラが実行されるかを知るためのマップを
必要とします。ルーティング設定のマップは、読みやすいフォーマットでこの情報を
提供します::

    # app/config/routing.yml
    blog_list:
        pattern:  /blog
        defaults: { _controller: AcmeBlogBundle:Blog:list }

    blog_show:
        pattern:  /blog/show/{id}
        defaults: { _controller: AcmeBlogBundle:Blog:show }

.. Now that Symfony2 is handling all the mundane tasks, the front controller
   is dead simple. And since it does so little, you'll never have to touch
   it once it's created (and if you use a Symfony2 distribution, you won't
   even need to create it!):

Symfony2 は全てのタスクを扱うようになり、フロントコントローラは完全に
シンプルになりました。フロントコントローラが行うことはとても少ないので、
一度作ったら最後、2度と触る必要はありません (Symfony2 ディストリビューションを
使う時には、わざわざ作る必要すらありません！) 。

.. code-block:: html+php

    <?php
    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->handle(Request::createFromGlobals())->send();

.. The front controller's only job is to initialize Symfony2's engine (``Kernel``)
   and pass it a ``Request`` object to handle. Symfony2's core then uses the
   routing map to determine which controller to call. Just like before, the
   controller method is responsible for returning the final ``Response`` object.
   There's really not much else to it.

フロントコントローラの唯一の仕事は、 Symfony2 のエンジン (``カーネル``) を
初期化し、 ``Request`` オブジェクトが取り扱えるよう渡すことです。
Symfony2 のコアはそれからどのコントローラを呼び出すか決めるため
ルーティングマップを使います。以前と同じように、コントローラのメソッドは
最終的な ``Response`` オブジェクトを返すことに責任を持っています。
それ以外には特にありません。

.. For a visual representation of how Symfony2 handles each request, see the
   :ref:`request flow diagram<request-flow-figure>`.

Symfony2 がそれぞれのリクエストをどのように取り扱うかのビジュアルな
説明は、 :ref:`request flow diagram<request-flow-figure>` を参照して
ください。

.. Where Symfony2 Delivers
   ~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 が提供するもの
~~~~~~~~~~~~~~~~~~~~~~~

.. In the upcoming chapters, you'll learn more about how each piece of Symfony
   works and the recommended organization of a project. For now, let's see how
   migrating the blog from flat PHP to Symfony2 has improved life:

次の章では、 Symfony のそれぞれの部分がどのように動くのかや、プロジェクトで
推奨される体系化の方法について学んでいきます。さしあたり、ブログをフラットな
PHP から Symfony2 に移行することがどのように生活の質を向上させるかを
見ましょう。

.. * Your application now has **clear and consistently organized code** (though
     Symfony doesn't force you into this). This promotes **reusability** and
     allows for new developers to be productive in your project more quickly.

* アプリケーションは **明確で一貫性のある体系付けられたコード** になりました
  (Symfony を通じてそう強要したわけではありません)。これは **再利用性** を
  高め、新しい開発者がプロジェクト内ですばやく生産的になれるようにします。

.. * 100% of the code you write is for *your* application. You **don't need
     to develop or maintain low-level utilities** such as :ref:`autoloading<autoloading-introduction-sidebar>`,
     :doc:`routing</book/routing>`, or rendering :doc:`controllers</book/controller>`.

* コードの100%全てが *あなたの* アプリケーションのものです。
  :ref:`autoloading<autoloading-introduction-sidebar>` や :doc:`routing</book/routing>` 、
  :doc:`controllers</book/controller>` のレンダリングといった
  **低レベルなユーティリティを開発したりメンテナンスする必要はありません**。

.. * Symfony2 gives you **access to open source tools** such as Doctrine and the
     Templating, Security, Form, Validation and Translation components (to name
     a few).

* Symfony2 は、 Doctrine や テンプレート、セキュリティ、フォーム、バリデーション、
  翻訳のコンポーネントといった **オープンソースのツールへのアクセス** を
  提供します。

.. * The application now enjoys **fully-flexible URLs** thanks to the ``Routing``
     component.

* アプリケーションは、 ``Routing`` コンポーネントのおかげで、 *完全に柔軟な URL*
  を実現しています。

.. * Symfony2's HTTP-centric architecture gives you access to powerful tools
     such as **HTTP caching** powered by **Symfony2's internal HTTP cache** or
     more powerful tools such as `Varnish`_. This is covered in a later chapter
     all about :doc:`caching</book/http_cache>`.

* Symfony2 の HTTP 中心のアーキテクチャは、**Symfony2 の内部 HTTP キャッシュ**
  を使って動作する **HTTP キャッシング**  や、さらにパワフルな `Varnish`_ の
  ようなツールへのアクセスを提供します。これは後で、:doc:`キャッシング</book/http_cache>`
  の全てで扱われます。

.. And perhaps best of all, by using Symfony2, you now have access to a whole
   set of **high-quality open source tools developed by the Symfony2 community**!
   For more information, check out `Symfony2Bundles.org`_

そして何よりも素晴らしいのは、 Symfony2 を使うことで、 **Symfony2 コミュニティに
よって開発された高品質なオープンソースツール** の集合全体へアクセスすることが
できるのです！ さらに詳しい情報は、 `Symfony2Bundles.org`_ を参照してください。

.. Better templates
   ----------------

よりよいテンプレート
--------------------

.. If you choose to use it, Symfony2 comes standard with a templating engine
   called `Twig`_ that makes templates faster to write and easier to read.
   It means that the sample application could contain even less code! Take,
   for example, the list template written in Twig:

Symfony2 を使うことに決めたら、 Symfony2 は 標準的に `Twig`_ と呼ばれる、
テンプレートの書き込みを早く、読み出しを簡単にするテンプレートエンジンが
同梱されてきます。これは、サンプルアプリケーションがさらに少ないコードで
動くことを意味しています！ 例として、 Twig で書かれた一覧表示の
テンプレートを挙げます。

.. code-block:: html+jinja

    {# src/Acme/BlogBundle/Resources/views/Blog/list.html.twig #}

    {% extends "::layout.html.twig" %}
    {% block title %}投稿のリスト{% endblock %}

    {% block body %}
        <h1>投稿のリスト</h1>
        <ul>
            {% for post in posts %}
            <li>
                <a href="{{ path('blog_show', { 'id': post.id }) }}">
                    {{ post.title }}
                </a>
            </li>
            {% endfor %}
        </ul>
    {% endblock %}

.. The corresponding ``layout.html.twig`` template is also easier to write:

対応する ``layout.html.twig`` テンプレートも同じく簡単に書くことが
できます。

.. code-block:: html+jinja

    {# app/Resources/views/layout.html.twig #}

    <html>
        <head>
            <title>{% block title %}デフォルトのタイトル{% endblock %}</title>
        </head>
        <body>
            {% block body %}{% endblock %}
        </body>
    </html>

.. Twig is well-supported in Symfony2. And while PHP templates will always
   be supported in Symfony2, we'll continue to discuss the many advantages of
   Twig. For more information, see the :doc:`templating chapter</book/templating>`.

Twig は Symfony2 でうまくサポートされています。そして、 PHP テンプレートが
常に Symfony2 でサポートされる一方で、 Twig の多くの長所についても議論を続けて
いくつもりです。詳しい情報は、 :doc:`テンプレートの章</book/templating>` を
参照してください。

.. Learn more from the Cookbook
   ----------------------------

クックブックからのより詳しい情報
--------------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/service`

.. _`Doctrine`: http://www.doctrine-project.org
.. _`symfony をダウンロード`: http://symfony.com/download
.. _`Routing`: https://github.com/symfony/Routing
.. _`Templating`: https://github.com/symfony/Templating
.. _`Symfony2Bundles.org`: http://symfony2bundles.org
.. _`Twig`: http://www.twig-project.org
.. _`Varnish`: http://www.varnish-cache.org
.. _`PHPUnit`: http://www.phpunit.de
