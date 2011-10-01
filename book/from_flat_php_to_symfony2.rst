.. 2011/05/07 doublemarket 88f8c07f

フラットな PHP が Symfony に出会う時
====================================

**なぜ Symfony2 は単にファイルを開いてフラットな PHP を書くよりも良いのでしょうか？**

もしあなたが PHP フレームワークを使ったことがなかったり、 MVC の哲学に親しみが
なかったり、あるいは Symfony2 周辺の *宣伝文句* が何なのか気になっただけ
だったりしたら、この章はあなたのためのものです。 Symfony2 がフラットな PHP を
使うよりも素早くよりよいソフトウェアを開発できるようにすることを *伝える*
ことなしに、あなた自身でそれが理解できるでしょう。

この章では、まずシンプルなアプリケーションをフラットな PHP で書きます。
それから、より体系づけられたものへ書き直していきます。タイムトラベルを
しながら、なぜ Web 開発が過去数年間にわたり現在の姿に発展してきたのかの
背景にある決断を見ていきます。

最後には、 Symfony2 がどのように一般的なタスクからあなたを解放し、自身の
コードのコントロールを取り戻すようお手伝いするかを知ることになります。

フラットな PHP による単純なブログ
---------------------------------

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

素早く書けて、高速に実行できますが、アプリケーションが大きくなるにつれて、
メンテナンスが手に負えなくなります。解決すべきいくつかの問題があります。

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

さあ、これらの問題を解決していきましょう。

表示部分の分離
~~~~~~~~~~~~~~

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

HTML コードは別のファイル (``templates/list.php``) に保存されるようになりました。
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

慣例によって、全てのアプリケーションのロジックを含むファイル「 ``index.php`` 」は
「コントローラ」と呼ばれます。 :term:`コントローラ` という用語は、あなたの使用する言語やフレームワークに関係なく、よく聞くことでしょう。
コントローラは、 *あなたの* コードにおける、ユーザからの入力を処理し、レスポンスを返す部分のことを指しています。

この場合、コントローラはデータベースからのデータを準備し、それからそのデータを提供する
テンプレートをインクルードします。テンプレートとコントローラを分離させることによって、
何か他のフォーマット (例えば JSON フォーマットの ``list.json.php``) で
ブログのエントリをレンダリングする必要があった場合に、テンプレートファイル *だけ*
を簡単に変更することができます。

アプリケーション (ドメイン) ロジックの分離
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

コントローラ (``index.php``) はとてもシンプルになります。

.. code-block:: html+php

    <?php

    require_once 'model.php';

    $posts = get_all_posts();

    require 'templates/list.php';

この時点で、コントローラの唯一のタスクは、アプリケーションのモデルレイヤー
(モデル) からデータを取り出し、そのデータをレンダリングするためにテンプレートを
呼び出すことです。これは、モデル-ビュー-コントローラパターンのとても単純な
例です。

レイアウトの分離
~~~~~~~~~~~~~~~~

この時点でアプリケーションは、いくつかの有利な点を持つ3つの明確な部品に
リファクタリングされ、別のページでほとんど全てを再利用できる機会を得ます。

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

ここで、レイアウトの再利用を可能にする方法を披露します。残念なことに、
これを可能にするために、いくつかの格好悪い PHP の関数 (``ob_start()`` と ``ob_end_clean()``)
をテンプレート内で使わなければならないことにお気づきだと思います。
Symfony2 はクリーンで簡単にこれを実現できる ``Templating`` コンポーネントを使います。
これはもうすぐ実践の中で見ていくことになります。

ブログの「show (単独表示) 」ページを追加
----------------------------------------

ブログの「list (一覧表示)」ページは、より体系付けられて再利用可能なコードに
なるようリファクタリングされました。これを証明するために、 ``id`` をクエリー
パラメータとしてそれぞれのブログの投稿を表示する、「show (単独表示)」ページを
追加しましょう。

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

次に、この新しいページのためのコントローラである ``show.php`` という
新しいファイルを作ってください。

.. code-block:: html+php

    <?php

    require_once 'model.php';

    $post = get_post_by_id($_GET['id']);

    require 'templates/show.php';

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

2番目のページを作るのは、とても簡単で、重複したコードもありません。まだ
このページには、フレームワークが解決できるさらにやっかいな問題があります。
例えば、「id」クエリーパラメータが存在しなかったり不正な場合、ページが
クラッシュする原因になります。このような問題では 404 ページを表示する方がよい
ですが、まだこれは簡単には実現できません。さらに問題なことに、
``mysql_real_escape_string()`` 関数を経由して ``id`` パラメータをクリーンに
し忘れると、データベース全体が SQL インジェクション攻撃のリスクにさらされる
ことになります。

それ以外の大きな問題として、それぞれのコントローラのファイルが ``model.php``
ファイルを含まなくてはならないということです。それぞれのコントローラファイルが、
突然追加のファイルを読み込む必要に迫られたり、その他のグローバルなタスク
(例えばセキュリティの向上など) を実行する必要が出た場合、どうなるでしょう。
現状では、それを実現するためのコードは全てのコントローラのファイルに追加する
必要があります。もし何かをあるファイルに含むのを忘れてしまった時、それが
セキュリティに関係ないといいのですが…。

「フロントコントローラ」の出番
------------------------------

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

.. tip::
    URI の ``index.php`` という一部分は、 Apache のリライトルール
    (あるいはそれと同等の仕組み) を使っている場合は、省略することが
    できます。この場合、ブログの単独表次ページの URI は、単純に
    ``/show`` になります。

フロントコントローラを使用する時は、一つの PHP ファイル (今回は ``index.php``) が
*全ての* リクエストをレンダリングします。ブログの単一表示ページでは、
``/index.php/show`` という URI で実際には、完全な URI に基づいてルーティングの
リクエストに内部的に応える ``index.php`` ファイルが実行されます。ここで見たように、
フロントコントローラはとてもパワフルなツールなのです。

フロントコントローラの作成
~~~~~~~~~~~~~~~~~~~~~~~~~~

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

ちょっと Symfony2 の考えを加える
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 の出番です。実際に Symfony2 を使う前に、 Symfony2 のクラスを
どのように見つけるのかを PHP が知っているようにする必要があります。
これは、 Symfony2 が提供するオートローダーを通じて実現されます。
オートローダーは、クラスを含むファイルを明確に含まなくても、 PHP のクラスを
使い始められるようにするツールです。

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

このファイルは、オートローダーに ``Symfony`` クラスがどこにあるかを知らせます。
これにより、 Symfony クラスを含むファイルで ``require`` ステートメントを
使わずに、 Symfony クラスを使い始めることができます。

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

Symfony2 の一部分を使うことによって、アプリケーションはより柔軟で
信頼できるものになりました。 ``Request`` は HTTP リクエストに関する情報に
アクセスするための信頼できる仕組みを提供します。具体的にいうと、
``getPathInfo()`` メソッドは整理された URI (常に ``/show`` で、
``/index.php/show`` ではない) を返します。そのため、もしユーザが ``/index.php/show``
にアクセスしたとしても、アプリケーションは ``show_action()`` によって
リクエストをルーティングするインテリジェントさを持っています。

``Response`` オブジェクトは、 HTTP ヘッダーとコンテンツをオブジェクト指向の
インタフェースを介して追加できるようにすることで、HTTP レスポンスを構成する際に
柔軟性を提供しています。そして、アプリケーションのレスポンスがシンプルな
ために、この柔軟性はアプリケーションが成長するのに大きな利点があるのです。

Symfony2でのサンプルアプリケーション
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ブログは *大きな* 成長をしてきました。しかし、まだこの程度の小さなアプリケーション
なのにたくさんのコードを含んでいます。ここに至るまで、単純なルーティング
システムや、テンプレートをレンダリングするため ``ob_start()`` と ``ob_end_clean()``
を使ったメソッドを開発してきました。もし、何らかの理由で、この「フレームワーク」を
作り続ける必要があるのなら、これらの問題を既に解決している Symfony のスタンドアローンの
`Routing`_ と `Templating`_ コンポーネントを最低でも使うことができたでしょう。

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

フロントコントローラの唯一の仕事は、 Symfony2 のエンジン (``カーネル``) を
初期化し、 ``Request`` オブジェクトが取り扱えるよう渡すことです。
Symfony2 のコアはそれからどのコントローラを呼び出すか決めるため
ルーティングマップを使います。以前と同じように、コントローラのメソッドは
最終的な ``Response`` オブジェクトを返すことに責任を持っています。
それ以外には特にありません。

Symfony2 がそれぞれのリクエストをどのように取り扱うかのビジュアルな
説明は、 :ref:`request flow diagram<request-flow-figure>` を参照して
ください。

.. Where Symfony2 Delivers
   ~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 が提供するもの
~~~~~~~~~~~~~~~~~~~~~~~

次の章では、 Symfony のそれぞれの部分がどのように動くのかや、プロジェクトで
推奨される体系化の方法について学んでいきます。さしあたり、ブログをフラットな
PHP から Symfony2 に移行することがどのように生活の質を向上させるかを
見ましょう。

* アプリケーションは **明確で一貫性のある体系付けられたコード** になりました
  (Symfony を通じてそう強要したわけではありません)。これは **再利用性** を
  高め、新しい開発者がプロジェクト内ですばやく生産的になれるようにします。

* コードの100%全てが *あなたの* アプリケーションのものです。
  :ref:`autoloading<autoloading-introduction-sidebar>` や :doc:`routing</book/routing>` 、
  :doc:`controllers</book/controller>` のレンダリングといった
  **低レベルなユーティリティを開発したりメンテナンスする必要はありません**。

* Symfony2 は、 Doctrine や テンプレート、セキュリティ、フォーム、バリデーション、
  翻訳のコンポーネントといった **オープンソースのツールへのアクセス** を
  提供します。

* アプリケーションは、 ``Routing`` コンポーネントのおかげで、 *完全に柔軟な URL*
  を実現しています。

* Symfony2 の HTTP 中心のアーキテクチャは、**Symfony2 の内部 HTTP キャッシュ**
  を使って動作する **HTTP キャッシング**  や、さらにパワフルな `Varnish`_ の
  ようなツールへのアクセスを提供します。これは後で、:doc:`キャッシング</book/http_cache>`
  の全てで扱われます。

そして何よりも素晴らしいのは、 Symfony2 を使うことで、 **Symfony2 コミュニティに
よって開発された高品質なオープンソースツール** の集合全体へアクセスすることが
できるのです！ さらに詳しい情報は、 `Symfony2Bundles.org`_ を参照してください。

よりよいテンプレート
--------------------

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

Twig は Symfony2 でうまくサポートされています。そして、 PHP テンプレートが
常に Symfony2 でサポートされる一方で、 Twig の多くの長所についても議論を続けて
いくつもりです。詳しい情報は、 :doc:`テンプレートの章</book/templating>` を
参照してください。

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
