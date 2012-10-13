.. 2012/10/13 brtriver f3474e18b34d3605a043e5b3a1ecc9ea099f370d

Symfony2 vs フラットなPHP
===============================

**なぜ Symfony2 は単にファイルを開いてフラットな PHP を書くよりも良いのでしょうか？**

この章は、まだ PHP のフレームワークを使ったことがない方や、MVC の哲学に慣れていない方、Symfony2 の特長が何なのかをすばやく知りたい方向けの解説です。\
Symfony2 を使うことで、フラットな PHP を使うよりも素早く、よりよいソフトウェアを開発できるということを、ご自身で１ステップずつ考えながら読み進めてください。

この章では、最初にフラットな PHP でシンプルなアプリケーションを記述します。

そのコードを出発点として、順に体系的なコードへリファクタリングしていきます。\
タイムトラベルをしながら、Web 開発が現在の姿に発展する過程や背景を見ていきましょう。

この章を読み終えると、Symfony2 を使うことで Web アプリケーション開発のどの部分が便利になっているのかを理解できるでしょう。\
そしてあなたのコードの支配権を、あなたの手に取り戻すことができるようになります。

フラットな PHP による単純なブログ
---------------------------------

この章では、フラットな PHP だけを使った外形だけのブログアプリケーションを作ります。\
初めに、データベースに永続化されたブログのエントリを表示する一つのページを作ってください。\
フラットな PHP を書くのは素早いけれど乱暴です。

.. code-block:: html+php

    <?php

    // index.php
    $link = mysql_connect('localhost', 'myuser', 'mypassword');
    mysql_select_db('blog_db', $link);

    $result = mysql_query('SELECT id, title FROM post', $link);

    ?>

    <!doctype html>
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
    ?>

素早く書けて高速に実行できますが、アプリケーションが大きくなるにつれてメンテナンスが手に負えなくなります。解決すべきいくつかの問題があります。

* **エラーチェックがありません** データベースへの接続が失敗した場合はどうなるのでしょう？

* **体系化されていません** アプリケーションが複雑になっていくと、この1つのファイルはどんどんメンテナンスできなくなっていきます。\
  フォームの送信を扱うコードはどこに入れたらよいでしょうか？\
  Eメールを送信するコードはどうしたらいいでしょうか？

* **コードの再利用が難しいです** 全てが1つのファイルにまとまっているので、アプリケーションの他の「ページ」のために一部を再利用する方法がありません。

.. note::

    ここで述べられていない他の問題として、データベースが MySQL に固定されてしまうということがあります。\
    ここではこの話に触れませんが、Symfony2 は `Doctrine`_ というデータベースの抽象化とマッピングを行うライブラリと完全に統合されています。

さあ、これらの問題を解決していきましょう。

表示部分の分離
~~~~~~~~~~~~~~

このコードは、HTML 表現を準備するコードからアプリケーションの「ロジック」を分離することで、すぐに改善できます。

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

HTML コードは別のファイル (``templates/list.php``) に保存されるようになりました。\
これは本来、テンプレート風の PHP 文法を使う HTML ファイルです。

.. code-block:: html+php

    <!doctype html>
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

慣例によって、全てのアプリケーションのロジックを含むファイル「\ ``index.php``\ 」は「コントローラ」と呼ばれます。\
:term:`コントローラ`\ という用語は、あなたの使用する言語やフレームワークに関係なく、よく聞くことでしょう。\
コントローラは、\ *あなたの*\ コードにおける、ユーザからの入力を処理し、レスポンスを返す部分のことを指しています。

この場合、コントローラはデータベースからのデータを準備し、それからそのデータを提供するテンプレートをインクルードします。\
テンプレートとコントローラを分離させることによって、何か他のフォーマット (例えば JSON フォーマットの ``list.json.php``) でブログのエントリをレンダリングする必要があった場合に、テンプレートファイル\ *だけ*\ を簡単に変更することができます。

アプリケーション (ドメイン) ロジックの分離
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

今のところアプリケーションは1つのページしか含んでいませんが、2番目のページが同じデータベース接続、あるいは同じ投稿の配列を使用する必要がある場合はどうでしょうか？\
アプリケーションのコアの動作とデータアクセスの機能が ``mode.php`` という新しいファイルに分離されるよう、コードをリファクタリングしてみましょう。

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

    ``model.php`` というファイル名が使われているのは、アプリケーションのロジックとデータアクセスが伝統的に「モデル」というレイヤーだからです。\
    うまく体系付けられたアプリケーションでは、「ビジネスロジック」を表すコードの大部分は、モデル内に存在するべきです (コントローラに存在するのとは対照的に) 。\
    そしてこの例とは違って、モデルの一部分のみが実際にデータベースへのアクセスに関わることになります。

コントローラ(``index.php``)はとてもシンプルになります。

.. code-block:: html+php

    <?php

    require_once 'model.php';

    $posts = get_all_posts();

    require 'templates/list.php';

この時点で、コントローラの唯一のタスクは、アプリケーションのモデルレイヤー(モデル)からデータを取り出し、そのデータをレンダリングするためにテンプレートを呼び出すことです。\
これは、モデル-ビュー-コントローラパターンのとても単純な例です。

レイアウトの分離
~~~~~~~~~~~~~~~~

この時点でアプリケーションは、いくつかの有利な点を持つ3つの明確な部品にリファクタリングされ、別のページでほとんど全てを再利用できる機会を得ます。

コードの中で再利用\ *できない*\ 唯一の部分は、ページレイアウトです。\
``layout.php`` ファイルを新しく作成して、これを修正しましょう。

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

レイアウトを「拡張」するようテンプレート(``templates/list.php``)を単純化できました。

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

ここで、レイアウトの再利用を可能にする方法を披露します。\
残念なことに、これを可能にするために、いくつかの格好悪い PHP の関数 (``ob_start()`` と ``ob_end_clean()``)をテンプレート内で使わなければならないことにお気づきだと思います。\
Symfony2 はクリーンで簡単にこれを実現できる ``Templating`` コンポーネントを使います。\
これはもうすぐ実践の中で見ていくことになります。

ブログの「show (単独表示) 」ページを追加
----------------------------------------

ブログの「list (一覧表示)」ページは、より体系付けられて再利用可能なコードになるようリファクタリングされました。\
これを証明するために、\ ``id`` をクエリーパラメータとしてそれぞれのブログの投稿を表示する「show (単独表示)」ページを追加しましょう。

まず初めに、与えられた ID を元にそれぞれのブログの結果を取得する関数を ``model.php`` ファイルに追加する必要があります。

.. code-block:: php

    // model.php
    function get_post_by_id($id)
    {
        $link = open_database_connection();

        $id = intval($id);
        $query = 'SELECT date, title, body FROM post WHERE id = '.$id;
        $result = mysql_query($query);
        $row = mysql_fetch_assoc($result);

        close_database_connection($link);

        return $row;
    }

次に、この新しいページのためのコントローラである ``show.php`` という新しいファイルを作ってください。

.. code-block:: html+php

    <?php

    require_once 'model.php';

    $post = get_post_by_id($_GET['id']);

    require 'templates/show.php';

最後に、それぞれの投稿を表示するための ``templates/show.php`` という新しいテンプレートファイルを作ってください。

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

2番目のページを作るのは、とても簡単で、重複したコードもありません。\
まだこのページには、フレームワークが解決できるさらにやっかいな問題があります。\
例えば、「id」クエリーパラメータが存在しなかったり不正な場合、ページがクラッシュする原因になります。\
このような問題では 404 ページを表示する方がよいですが、まだこれは簡単には実現できません。\
さらに問題なことに、\ ``intval()`` 関数を経由して ``id`` パラメータをクリーンにし忘れると、データベース全体が SQL インジェクション攻撃のリスクにさらされることになります。

それ以外の大きな問題として、それぞれのコントローラのファイルが ``model.php`` ファイルを含まなくてはならないということです。\
それぞれのコントローラファイルが、突然追加のファイルを読み込む必要に迫られたり、その他のグローバルなタスク(例えばセキュリティの向上など)を実行する必要が出た場合、どうなるでしょう。\
現状では、それを実現するためのコードは全てのコントローラのファイルに追加する必要があります。\
もし何かをあるファイルに含むのを忘れてしまった時、それがセキュリティに関係ないといいのですが…。

「フロントコントローラ」の出番
------------------------------

解決策は、フロントコントローラを使うことです。\
これは、\ *全ての*\ リクエストが処理される際に通過する一つの PHP ファイルです。\
フロントコントローラによって、アプリケーションの URI は少し変更されますが、より柔軟になり始めます。

.. code-block:: text

    フロントコントローラなしの場合
    /index.php          => ブログ一覧表示ページ (index.php が実行されます)
    /show.php           => ブログ単独表示ページ (show.php が実行されます)

    index.php をフロントコントローラとして使用した場合
    /index.php          => ブログ一覧表示ページ (index.php が実行されます)
    /index.php/show     => ブログ単独表示ページ (index.php が実行されます)

.. tip::
    URI の ``index.php`` という一部分は、Apache のリライトルール(あるいはそれと同等の仕組み)を使っている場合は、省略することができます。\
    この場合、ブログの単独表示ページの URI は、単純に ``/show`` になります。

フロントコントローラを使用する時は、一つの PHP ファイル(今回は ``index.php``)が\ *全ての*\ リクエストをレンダリングします。\
ブログの単一表示ページでは、\ ``/index.php/show`` という URI で実際には、完全な URI に基づいてルーティングのリクエストに内部的に応える ``index.php`` ファイルが実行されます。\
ここで見たように、フロントコントローラはとてもパワフルなツールなのです。

フロントコントローラの作成
~~~~~~~~~~~~~~~~~~~~~~~~~~

我々のアプリケーションに関して、\ **大きな**\ 一歩を踏み出そうとしています。\
全てのリクエストを扱う一つのファイルによって、セキュリティの扱いや、設定の読み込み、ルーティングといったことを集中的に扱えるようになります。\
我々のアプリケーションでは ``index.php`` が、リクエストされた URI に基づいて、ブログの一覧表示ページ\ *あるいは*\ 単一表示ページをレンダリングするのに十分なぐらい洗練されている必要があります。

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

コードの体系化のために、2つのコントローラ(以前の index.php と show.php)は、PHP の関数になり、それぞれは別のファイル controllers.php に移動されました。

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

フロントコントローラとして、\ ``index.php`` は全く新しい役割を引き受けることになりました。\
それは、コアライブラリを読み込み、2つのコントローラ(``list_action()`` と ``show_action()`` 関数)のうちの1つを呼び出せるようにアプリケーションをルーティングすることです。\
実際にこのフロントコントローラは、リクエストを取り扱いルーティングする Symfony2 のメカニズムによく似た見た目と動作をし始めています。

.. tip::

   フロントコントローラのもう一つの利点が、柔軟性のある URL です。\
   コードのたった1箇所だけを変更すれば、ブログ単一表示ページの URL を ``/show`` から ``/read`` に変更できることに注目してください。\
   以前は、ファイル全体の名前を変更する必要がありましたね。\
   Symfony2 では、URL の取り扱いはもっとずっと柔軟性があります。

ここまで、アプリケーションを単一の PHP ファイルから、体系化されてコードの再利用ができる構造へと発展させてきました。\
これでハッピーになるべきですが、満足からは程遠いでしょう。\
例えば、「ルーティング」システムは気まぐれで、一覧表示ページ(/index.php)が / (Apacheのリライトルールが追加されている場合)からでもアクセス可能であるべきだということを認識できません。\
また、ブログを開発する代わりに、コードの「アーキテクチャ」(例えばルーティングや呼び出すコントローラ、テンプレートなど)にたくさんの時間を費やしています。\
より多くの時間を、フォームの送信の扱い、入力のバリデーション、ロギングやセキュリティといったことに費やす必要があるでしょう。\
なぜこれら全てのありふれた問題への解決策を再発明しなければならないのでしょうか？

ちょっと Symfony2 の考えを加える
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 の出番です。\
実際に Symfony2 を使う前に、Symfony2 のクラスをどのように見つけるのかを PHP が知っているようにする必要があります。\
これは、 Symfony2 が提供するオートローダーを通じて実現されます。\
オートローダーは、クラスを含むファイルを明確に含まなくても、 PHP のクラスを使い始められるようにするツールです。

まず最初に、\ `Symfony をダウンロード`_\ し、\ ``vendor/symfony/symfony`` ディレクトリに配置してください。\
次に、\ ``app/bootstrap.php`` ファイルを作ってください。\
アプリケーション内の2つのファイルを\ ``要求``\ し、オートローダーを設定するためにこのファイルを使います。

.. code-block:: html+php

    <?php
    // bootstrap.php
    require_once 'model.php';
    require_once 'controllers.php';
    require_once 'vendor/symfony/symfony/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';

    $loader = new Symfony\Component\ClassLoader\UniversalClassLoader();
    $loader->registerNamespaces(array(
        'Symfony' => __DIR__.'/../vendor/symfony/symfony/src',
    ));

    $loader->register();

このファイルは、オートローダーに ``Symfony`` クラスがどこにあるかを知らせます。\
これにより、Symfony クラスを含むファイルで ``require`` ステートメントを使わずに、Symfony クラスを使い始めることができます。

Symfony の哲学の核は、アプリケーションの主なジョブはそれぞれのリクエストを解釈し、レスポンスを返すことであるという考え方です。\
この目的のために、Symfony2 は :class:`Symfony\\Component\\HttpFoundation\\Request` と :class:`Symfony\\Component\\HttpFoundation\\Response` という2つのクラスを提供しています。\
これらのクラスは、処理されるべき生の HTTP リクエストと、返される HTTP レスポンスのオブジェクト指向での実装になっています。\
ブログを改善するために、これらを使いましょう。

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

コントローラは、\ ``Response`` オブジェクトを返す責任を持つようになりました。\
これを簡単にするために、新しく ``render_template()`` 関数を追加できます。\
ちなみに、この関数は Symfony2 のテンプレートエンジンとちょっと似た動きをします。

.. code-block:: php

    // controllers.php
    use Symfony\Component\HttpFoundation\Response;

    function list_action()
    {
        $posts = get_all_posts();
        $html = render_template('templates/list.php', array('posts' => $posts));

        return new Response($html);
    }

    function show_action($id)
    {
        $post = get_post_by_id($id);
        $html = render_template('templates/show.php', array('post' => $post));

        return new Response($html);
    }

    // テンプレートをレンダリングするためのヘルパー関数
    function render_template($path, array $args)
    {
        extract($args);
        ob_start();
        require $path;
        $html = ob_get_clean();

        return $html;
    }

Symfony2 の一部分を使うことによって、アプリケーションはより柔軟で信頼できるものになりました。\
``Request`` は HTTP リクエストに関する情報にアクセスするための信頼できる仕組みを提供します。\
具体的にいうと、\ ``getPathInfo()`` メソッドは整理された URI(常に ``/show`` で、\ ``/index.php/show`` ではない)を返します。\
そのため、もしユーザが ``/index.php/show`` にアクセスしたとしても、アプリケーションは ``show_action()`` によってリクエストをルーティングするインテリジェントさを持っています。

``Response`` オブジェクトは、HTTP ヘッダーとコンテンツをオブジェクト指向のインタフェースを介して追加できるようにすることで、HTTP レスポンスを構成する際に柔軟性を提供しています。\
そして、アプリケーションのレスポンスがシンプルなために、この柔軟性はアプリケーションが成長するのに大きな利点があるのです。

Symfony2でのサンプルアプリケーション
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ブログは\ *大きな*\ 成長をしてきました。\
しかし、まだこの程度の小さなアプリケーションなのにたくさんのコードを含んでいます。\
ここに至るまで、単純なルーティングシステムや、テンプレートをレンダリングするため ``ob_start()`` と ``ob_get_clean()`` を使ったメソッドを開発してきました。\
もし、何らかの理由でこの「フレームワーク」を作り続ける必要があるのなら、これらの問題を既に解決している Symfony のスタンドアローンの `Routing`_ と `Templating`_ コンポーネントを使うこともできるでしょう。

一般的な問題を改めて解決する代わりに、Symfony2 にそれらの面倒を見させることができます。\
以下が Symfony2 を使った同じサンプルアプリケーションです。

.. code-block:: html+php

    <?php
    // src/Acme/BlogBundle/Controller/BlogController.php
    namespace Acme\BlogBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class BlogController extends Controller
    {
        public function listAction()
        {
            $posts = $this->get('doctrine')->getManager()
                ->createQuery('SELECT p FROM AcmeBlogBundle:Post p')
                ->execute();

            return $this->render('AcmeBlogBundle:Blog:list.html.php', array('posts' => $posts));
        }

        public function showAction($id)
        {
            $post = $this->get('doctrine')
                ->getManager()
                ->getRepository('AcmeBlogBundle:Post')
                ->find($id);

            if (!$post) {
                // cause the 404 page not found to be displayed
                throw $this->createNotFoundException();
            }
 
            return $this->render('AcmeBlogBundle:Blog:show.html.php', array('post' => $post));
        }
    }

2つのコントローラはまだ軽量です。\
それぞれ、データベースからオブジェクトを取り出すために Doctrine ORM ライブラリを使用し、テンプレートをレンダリングして ``Response`` オブジェクトを返すために ``Templating`` コンポーネントを使用しています。\
一覧表示のテンプレートは少しシンプルになりました。

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

レイアウトはほとんど全く同じです。

.. code-block:: html+php

    <!-- app/Resources/views/layout.html.php -->
    <!doctype html>
    <html>
        <head>
            <title><?php echo $view['slots']->output('title', 'デフォルトのタイトル') ?></title>
        </head>
        <body>
            <?php echo $view['slots']->output('_content') ?>
        </body>
    </html>

.. note::

    単一表示のテンプレートはエクササイズとして残しておきます。\
    一覧表示のテンプレートを元にして作成するのは簡単なはずです。

Symfony2 のエンジン(``カーネル`` と呼ばれます)が起動する時には、リクエスト情報を元にどのコントローラが実行されるかを知るためのマップを必要とします。\
ルーティング設定のマップは、読みやすいフォーマットでこの情報を提供します。

.. code-block:: yaml

    # app/config/routing.yml
    blog_list:
        pattern:  /blog
        defaults: { _controller: AcmeBlogBundle:Blog:list }

    blog_show:
        pattern:  /blog/show/{id}
        defaults: { _controller: AcmeBlogBundle:Blog:show }

Symfony2 は全てのタスクを扱うようになり、フロントコントローラは完全にシンプルになりました。\
フロントコントローラが行うことはとても少ないので、一度作ったら最後、2度と触る必要はありません(Symfony2 ディストリビューションを使う時には、わざわざ作る必要すらありません！) 。

.. code-block:: html+php

    <?php
    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->handle(Request::createFromGlobals())->send();

フロントコントローラの唯一の仕事は、Symfony2 のエンジン(``カーネル``)を初期化し、\ ``Request`` オブジェクトが取り扱えるよう渡すことです。\
Symfony2 のコアはそれからどのコントローラを呼び出すか決めるためルーティングマップを使います。\
以前と同じように、コントローラのメソッドは最終的な ``Response`` オブジェクトを返すことに責任を持っています。
それ以外には特にありません。

Symfony2 がそれぞれのリクエストをどのように取り扱うかのビジュアルな説明は、\ :ref:`request flow diagram<request-flow-figure>` を参照してください。

.. Where Symfony2 Delivers
   ~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 が提供するもの
~~~~~~~~~~~~~~~~~~~~~~~

次の章では、 Symfony のそれぞれの部分がどのように動くのかや、プロジェクトで推奨される体系化の方法について学んでいきます。\
さしあたり、ブログをフラットな PHP から Symfony2 に移行することがどのように生活の質を向上させるかを見ましょう。

* アプリケーションは\ **明確で一貫性のある体系付けられたコード**\ になりました(Symfony を通じてそう強要したわけではありません)。\
  これは\ **再利用性**\ を高め、新しい開発者がプロジェクト内ですばやく生産的になれるようにします。

* コードの100%全てが\ **あなたの**\ アプリケーションのものです。\
  :ref:`オートロード<autoloading-introduction-sidebar>`\ や\ :doc:`ルーティング</book/routing>`\ 、\ :doc:`コントローラ</book/controller>`\ のレンダリングといった\ **低レベルなユーティリティを開発したりメンテナンスする必要はありません**\ 。

* Symfony2 は、Doctrine や テンプレート、セキュリティ、フォーム、バリデーション、翻訳のコンポーネントといった\ **オープンソースのツールへのアクセス**\ を提供します。

* アプリケーションは、\ ``Routing`` コンポーネントのおかげで、\ *完全に柔軟な URL* を実現しています。

* Symfony2 の HTTP 中心のアーキテクチャは、\ **Symfony2 の内部 HTTP キャッシュ**\ を使って動作する **HTTP キャッシング**\ や、さらにパワフルな `Varnish`_ のようなツールへのアクセスを提供します。\
  これは後で、\ :doc:`キャッシング</book/http_cache>`\ の全てで扱われます。

そして何よりも素晴らしいのは、Symfony2 を使うことで\ **Symfony2 コミュニティによって開発された高品質なオープンソースツール**\ の集合全体へアクセスすることができるのです！\
さらに詳しい情報は、\ `Symfony2Bundles.org`_ を参照してください。

よりよいテンプレート
--------------------

Symfony2 を使うことに決めたら、Symfony2 は 標準的に `Twig`_ と呼ばれる、テンプレートの書き込みを早く、読み出しを簡単にするテンプレートエンジンが同梱されてきます。\
これは、サンプルアプリケーションがさらに少ないコードで動くことを意味しています！\
例として、Twig で書かれた一覧表示のテンプレートを挙げます。

.. code-block:: html+jinja

    {# src/Acme/BlogBundle/Resources/views/Blog/list.html.twig #}
    {% extends "::layout.html.twig" %}
    {% block title %}投稿のリスト{% endblock %}

    {% block body %}
        <h1>投稿のリスト</h1>
        <ul>
            {% for post in posts %}
            <li>
                <a href="{{ path('blog_show', {'id': post.id}) }}">
                    {{ post.title }}
                </a>
            </li>
            {% endfor %}
        </ul>
    {% endblock %}

.. The corresponding ``layout.html.twig`` template is also easier to write:

対応する ``layout.html.twig`` テンプレートも同じく簡単に書くことができます。

.. code-block:: html+jinja

    {# app/Resources/views/layout.html.twig #}

    <!doctype html>
    <html>
        <head>
            <title>{% block title %}デフォルトのタイトル{% endblock %}</title>
        </head>
        <body>
            {% block body %}{% endblock %}
        </body>
    </html>

Twig は Symfony2 でうまくサポートされています。\
そして、PHP テンプレートが常に Symfony2 でサポートされる一方で、Twig の多くの長所についても議論を続けていくつもりです。\
詳しい情報は、\ :doc:`テンプレートの章</book/templating>`\ を参照してください。

クックブックからのより詳しい情報
--------------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/service`

.. _`Doctrine`: http://www.doctrine-project.org
.. _`Symfony をダウンロード`: http://symfony.com/download
.. _`Routing`: https://github.com/symfony/Routing
.. _`Templating`: https://github.com/symfony/Templating
.. _`KnpBundles.com`: http://knpbundles.com/
.. _`Twig`: http://twig.sensiolabs.org
.. _`Varnish`: http://www.varnish-cache.org
.. _`PHPUnit`: http://www.phpunit.de
