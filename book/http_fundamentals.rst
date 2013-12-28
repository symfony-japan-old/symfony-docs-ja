.. index::
   single: Symfony2 Fundamentals

.. note::

    * 対象バージョン：2.4 (2.0以降)
    * 翻訳更新日：2013/12/28

Symfony2 と HTTP の基礎
=======================

Symfony2 は、ソフトウェアの基本を大切にしています。
迅速で堅牢なアプリケーション開発を可能にしながらも、開発者にフレームワークのルールを押し付けたりしません。
また、Symfony は、さまざまな技術的知見を基に作られています。\
たくさんの人々が長年積み上げてきた知恵の結晶を、Symfony のツールやコンセプトから学ぶことができます。

この章では Web 開発に共通した基礎的なコンセプトである、HTTP を説明することから始めましょう。
どんな経歴を持っていても、どんなプログラミング言語が好きでも、この章はすべての人にとって\ **必読**\ です。

HTTP はシンプル
---------------

HTTP (Hypertext Transfer Protocol) は、2 台のマシンがお互いにやり取りするためのテキスト言語です。
例えば、Web コミックの `xkcd`_ 最新情報をチェックしたいとしましょう。
その場合、おおよそこんな会話が起こっています。

.. figure:: /images/http-xkcd.png
   :align: center

   browser->server「おーい！今日の分ある？」/
   server: ページの HTML を用意する /
   server->browser「あるよ！HTML あげるね。」

実際にはもう少し形式的ですが、本当にシンプルです。
HTTP は、このシンプルなテキストベースの言語を記述するための規約です。
Web の開発ということであれば、サーバーの役割はテキストのリクエストを解釈し、\
テキストのレスポンスを返すことです。

HTTP のやりとりを順に見て行きましょう。

.. index::
   single: HTTP; Request-response paradigm

ステップ 1: クライアントがリクエストを送る
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

WEB 上でのやりとりは、どんな時でも\ *リクエスト*\ から始まります。
リクエストとは、クライアント(ブラウザやiPhoneアプリ等)が作成するテキストメッセージで、HTTP として知られている特別なフォーマットを使って作成されます。
クライアントは、サーバーにリクエストを送り、レスポンスを待ちます。

ブラウザと xkcd の WEB サーバーとのやりとりのうち、最初の部分(リクエスト)を見てみましょう。

.. figure:: /images/http-xkcd-request.png
   :align: center

   browser->server「おーい！今日の分ある？」

HTTP において、リクエストは次のようになります。

.. code-block:: text

    GET / HTTP/1.1
    Host: xkcd.com
    Accept: text/html
    User-Agent: Mozilla/5.0 (Macintosh)

このシンプルなメッセージだけで、クライアントがどのリソースをリクエストしてきているのか、必要なことが\ *すべて*\ わかります。
1 行目が最も重要で、URI と HTTP メソッドが記載されています。

URI (``/`` や ``/contact`` など) は、クライアントが必要としているリソースのユニークなアドレス/場所です。
HTTP メソッド (``GET`` など) は、そのリソースに対して\ *どうしたい*\ のか定義します。
HTTP メソッドはリクエストの\ *動詞*\ にあたり、そのリソースに対してどのように振る舞いたいのかを限定された操作によって定義します。

+----------+-----------------------------------------+
| *GET*    | サーバーからリソースを読み込む          |
+----------+-----------------------------------------+
| *POST*   | サーバー上でリソースを作成する          |
+----------+-----------------------------------------+
| *PUT*    | サーバー上のリソースをアップデートする  |
+----------+-----------------------------------------+
| *DELETE* | サーバー上のリソースを削除する          |
+----------+-----------------------------------------+

上記を考慮すると、例えばブログのとあるエントリを削除する HTTP リクエストはこうなります。

.. code-block:: text

    DELETE /blog/15 HTTP/1.1

.. note::

    HTTP 仕様には 9 個の HTTP メソッドが定義されていますが、\
    多くはそれほど広く使われておらず、サポートもされていません。
    実際、最近のブラウザでも ``PUT`` や ``DELETE`` はサポートされていません。

HTTP リクエストは、1 行目の後に、必ずリクエストヘッダと呼ばれる行が複数行続きます。
ヘッダーには幅広い範囲の情報を与えることができ、例えば、\
リクエスト元の ``Host`` や、クライアントが ``Accept`` できるレスポンスフォーマット、リクエストを作成する際に使用したアプリケーション(\ ``User-Agent``\ )などの情報を与えることができるようになっています。
ヘッダーは他にもたくさんあるので、Wikipedia の `List of HTTP header fields`_ の記事を参照してみてください。

ステップ 2: サーバーがレスポンスを返す
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

サーバーはリクエストを受け取ると、URI を通じて、クライアントが何を必要としているのか、また、メソッドを通じて、そのリソースに対してどうしたいのかを知ることができます。
例えば、GET リクエストであれば、サーバーはリソースを用意し、HTTP レスポンスとして返します。
xkcd WEB サーバーのレスポンスを見てみましょう。

.. figure:: /images/http-xkcd.png
   :align: center

   browser->server「おーい！今日分ある？」/
   server: HTMLを用意する /
   server->browser「あるよ！HTMLあげるね。」

HTTP では、次のようなレスポンスを返すことに相当します。

.. code-block:: text

    HTTP/1.1 200 OK
    Date: Sat, 02 Apr 2011 21:05:05 GMT
    Server: lighttpd/1.4.19
    Content-Type: text/html

    <html>
      <!-- ... xkcd コミックのHTML -->
    </html>

HTTP レスポンスには、リクエストされたリソース(この場合では HTML コンテンツ)、および、レスポンスに関する情報が含まれています。
1 行目が特に重要で、HTTP レスポンスのステータスコード(ここでは 200)が記載されます。
ステータスコードは、リクエストがどのような結果になったのかをクライアントに伝える役割を果たします。
リクエストは成功したのか？エラーがあったのか？
ステータスコードには、成功、エラー、クライアントがすべきこと(リダイレクトで別ページに遷移する等)の意味が割り振られています。
Wikipedia の `List of HTTP status codes`_ に、ステータスコードのリストがありますので、参照してみてください。

リクエストと同様に、HTTP レスポンスには、HTTP ヘッダーとして、付加的な情報が含まれています。
例えば、HTTP レスポンスヘッダーとして重要なものに、\ ``Content-Type`` があります。
同じリソースを返すにしても、例えば HTML や XML、JSON といった様々な返し方があります。
``Content-Type`` ヘッダーでは、\ ``text/html`` といったインターネットメディアタイプにより、返されるフォーマットの種類をクライアントに知らせます。
一般的なメディアタイプの一覧は、Wikipedia の `List of common media types`_ を参照してください。

他にもたくさんのヘッダーがあります。\
例えば、強力なキャッシュシステムのために使われるヘッダもあります。


リクエスト、レスポンス、そして Web 開発
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

このリクエスト-レスポンス間のやりとりは、Web 上で通信を行う上で基本的なプロセスとなります。
このプロセスは、重要で強力であると同時に、必然的にシンプルです。

もっとも重要なのは、\
どんなプログラミング言語を使っていようとも、どんなアプリケーション(Web、モバイル、JSON API)を作ろうとも、そして、どんな開発方針に従っていようとも、\
アプリケーションの最終的な目的は、リクエストを解釈して適切なレスポンスを返すことにある、ということです。

Symfony は、この事実に応えられるように設計されています。

.. tip::

    HTTP の仕様についてより詳しく知りたければ、オリジナルの `HTTP 1.1 RFC`_ を読んでみてください。
    もしくは、オリジナル仕様を積極的に明確化している `HTTP Bis`_ を読んでみてもいいでしょう。
    `Live HTTP Headers`_ という、ブラウジング中のリクエスト/レスポンスヘッダを検証する Firefox のエクステンションもあります。

.. index::
   single: Symfony2 Fundamentals; Requests and responses

PHP におけるリクエストとレスポンス
----------------------------------

それでは PHP を使った場合、「リクエスト」や「レスポンス」はどのように実装するのでしょうか。
PHP を使うと、作業は少し簡単になります。

.. code-block:: php

    $uri = $_SERVER['REQUEST_URI'];
    $foo = $_GET['foo'];

    header('Content-type: text/html');
    echo 'リクエストされたURIは '.$uri;
    echo 'パラメータ "foo" の中身は '.$foo;

奇妙に聞こえるかもしれませんが、上記の例では、\
HTTP リクエストから情報を取得し、その情報を使って HTTP レスポンスを組み立てています。
PHP では、わざわざ HTTP リクエストをパースしなくても、\
``$_SERVER`` や ``$_GET`` のようなスーパーグローバル変数にリクエストの情報がすべて格納されています。
同様に、HTTP プロトコルに従ったテキストレスポンスを返さなくても、\
``header()`` 関数を使用してレスポンスヘッダを送信し、\
コンテンツ内容部のみを出力すれば、\
PHP が正しいフォーマットの HTTP レスポンスを作成し、クライアントに返されます。

.. code-block:: text

    HTTP/1.1 200 OK
    Date: Sat, 03 Apr 2011 02:14:33 GMT
    Server: Apache/2.2.17 (Unix)
    Content-Type: text/html

    リクエストされたURIは /testing?foo=symfony
    パラメータ "foo" の中身は symfony

Symfony におけるリクエストとレスポンス
--------------------------------------

Symfony では、先ほど説明した PHP よる直接的なアプローチの代わりに、HTTP のリクエストとレスポンスを手軽に扱える 2 つのクラスを使います。
:class:`Symfony\\Component\\HttpFoundation\\Request` クラスは、HTTP リクエストメッセージをシンプルなオブジェクト指向で表現したクラスです。
これを使えば、すべてのリクエスト情報が簡単に手に入ります。

.. code-block:: php

    use Symfony\Component\HttpFoundation\Request;

    $request = Request::createFromGlobals();

    // リクエストされたURI(例: /about)
    // ただし、クエリパラメータはすべて除去されます
    $request->getPathInfo();

    // それぞれ GET 変数と POST 変数を取得
    $request->query->get('foo');
    $request->request->get('bar', 'bar が存在しない場合のデフォルト値');

    // SERVER 変数を取得
    $request->server->get('HTTP_HOST');

    // foo で指定した UploadedFile オブジェクトの取得
    $request->files->get('foo');

    // COOKIE の値を取得
    $request->cookies->get('PHPSESSID');

    // HTTP リクエストヘッダーの値を取得（正規化、小文字化したキーを使用）
    $request->headers->get('host');
    $request->headers->get('content_type');

    $request->getMethod();          // GET, POST, PUT, DELETE, HEAD
    $request->getLanguages();       // クライアントが許可している言語のリスト

``Request`` クラスでは、開発者の手を煩わせないように様々な処理も行われます。
例えば、\ ``isSecure()`` メソッドは、 PHP 内の\ *3 つ*\ の値をチェックしていて、\
ユーザーがセキュアな接続(\ ``HTTPS``\ )を利用しているのかどうかを確認することができます。

.. sidebar:: ParameterBags と Request の属性

    上で見たように、\ ``$_GET`` と ``$_POST`` 変数には、それぞれ ``query`` および ``request`` というパブリックなプロパティでアクセスできます。
    これらはいずれも :class:`Symfony\\Component\\HttpFoundation\\ParameterBag` オブジェクトで、
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::get`\ 、
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::has`\ 、
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::all` などのメソッドを持っています。
    上の例にあるパブリックプロパティも ParameterBag のインスタンスです。

    .. _book-fundamentals-attributes:

    Request クラスには ``attributes`` というパブリックプロパティもあります。
    このプロパティには、アプリケーションで内部的に利用される特殊なデータが格納されます。
    Symfony2 フレームワークでは、マッチしたルートにおける ``_controller`` の値や、ルートに ``{id}`` ワイルドカードがある場合は ``id`` の値、およびマッチしたルートの名前(``_route``)が ``attributes`` プロパティに格納されます。
    ``attributes`` プロパティは、リクエストに関するコンテキスト独自の情報を準備して格納する場所として使うこともできます。

Symfony は、HTTP レスポンスメッセージを表す ``Response`` クラスも提供しています。
オブジェクト指向インターフェイスで、クライアントに返すレスポンスを作成することができます。

.. code-block:: php

    use Symfony\Component\HttpFoundation\Response;
    $response = new Response();

    $response->setContent('<html><body><h1>Hello world!</h1></body></html>');
    $response->setStatusCode(Response::HTTP_OK);
    $response->headers->set('Content-Type', 'text/html');

    // HTTP ヘッダを出力し、続いてコンテンツも出力する
    $response->send();

.. versionadded:: 2.4
    Support for HTTP status code constants was added in Symfony 2.4.

もし Symfony にこれ以上の機能がなかったとしても、\
リクエスト情報に容易にアクセス可能な、\
そして、オブジェクト指向インターフェイスでレスポンス作成が可能なツールキット\
を手にしたことになります。
Symfony のもっと強力な機能を勉強したとしても、次のことだけは心に留めておいて下さい。
アプリケーションの目標は、常に、\ *リクエストを解釈し、ロジックに基づき、適切なレスポンスを作成すること*\ にあります。

.. tip::

    ``Request`` クラスと ``Response`` クラスは、HttpFoundation という
    Symfony 付属のスタンドアロンなコンポーネントに含まれています。
    このコンポーネントは、Symfony とは独立して利用できます。
    他にも、セッションやファイルアップロードを扱うクラスもあります。

Request から Response ができるまで
----------------------------------

HTTP それ自体がそうであるように、\ ``Request`` と ``Response`` オブジェクトは本当にシンプルです。
アプリケーションを作る上で一番大変なのは、それらの「間」を実装することです。
つまり、本来やるべき作業は、リクエストを解釈し、レスポンスを作成するコードを書くこと、ということなのです。

アプリケーションは、時に、メールを送ったり、フォームの送信を受け付けたり、\
データベースに値を保存したり、HTML ページをレンダリングしたり、\
コンテンツをセキュアにしておいたりすることがありますが、\
これらを達成し、整ったコードを保ち、そして保守し続けることが可能なようにするにはどうしたら良いのでしょうか。

フロントコントローラー
~~~~~~~~~~~~~~~~~~~~~~

従来、アプリケーションは、サイト内の各「ページ」に物理的なファイルを置くように作られてきました。

.. code-block:: text

    index.php
    contact.php
    blog.php

この方針のままでは、いくつか問題が生じてきます。
例えば、URL の柔軟性についてです。
「\ ``blog.php`` から ``news.php`` にファイル名を変更したいけど、リンクは壊したくない。」
こんな場合はどうしたら良いでしょうか。
また、各ファイルは、セキュリティの確保やデータベース接続、サイトの「見た目」を一貫したものにするために、\
コアファイルを\ *例外なく*\ いちいちインクルードしているという点があります。

:term:`フロントコントローラー`\ を使うのは、良い解決方法でしょう。
アプリケーションへのすべてのリクエストを、1つの PHP ファイルで一手に引き受けるのです。
たとえば、次のようになります。

+------------------------+--------------------------+
| ``/index.php``         | ``index.php`` を実行する |
+------------------------+--------------------------+
| ``/index.php/contact`` | ``index.php`` を実行する |
+------------------------+--------------------------+
| ``/index.php/blog``    | ``index.php`` を実行する |
+------------------------+--------------------------+

.. tip::

    Apache の ``mod_rewrite`` (Apache以外でも、それに相当するもの) を使えば、\
    URLを ``/`` や ``/contact`` や ``/blog`` のように短縮することは簡単です。

さて、すべてのリクエストが全く同じように処理されるようになりました。
URL によって別々の PHP ファイルが実行されるのではなく、\ *常に*\ フロントコントローラーが実行されます。
フロントコントローラーの内部で、URL に応じた処理にルーティングされるのです。
こうしておけば、先の 2 つの問題は解決されます。
モダンな Web アプリケーションでは、ほとんどこのアプローチをとっていて、例えば WordPress のようなアプリケーションも同様です。

整ったコードを保つ
~~~~~~~~~~~~~~~~~~

フロントコントローラーの中では、どのコードを実行し、どのコンテンツを返すのかを指示する必要があります。
リクエストされた URI をチェックし、URI の値に応じたコードを実行するようにします。
これを文字通りのコードにするのは簡単ですが、すぐに手を付けられないコードになることは明白です。

.. code-block:: php

    // index.php
    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpFoundation\Response;

    $request = Request::createFromGlobals();
    $path = $request->getPathInfo(); // リクエストされた URI パス

    if (in_array($path, array('', '/'))) {
        $response = new Response('Welcome to the homepage.');
    } elseif ($path == '/contact') {
        $response = new Response('Contact us');
    } else {
        $response = new Response('Page not found.', Response::HTTP_NOT_FOUND);
    }
    $response->send();

Symfony による解決法を見ていきましょう。

Symfony アプリケーションのフロー
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony がリクエストを扱えるようになれば、人生楽しくなります。
Symfony はすべてのリクエストで、同じシンプルなパターンを踏みます。

.. _request-flow-figure:

.. figure:: /images/request-flow.png
   :align: center
   :alt: Symfony2 request flow

   入ってきたリクエストは、ルーティングにより解釈された後、\
   コントローラ関数に渡され、 ``Response`` オブジェクトを返す。

「ページ」は、ルーティングの設定ファイルで定義します。
URL を PHP に割り振るというわけです。
この割り当てられた PHP の処理を、\ :term:`コントローラー`\ と呼び、\
リクエストの情報を使用し、また同時に Symfony により使用可能になる様々なツールも使用して、\
``Response`` オブジェクトを作成して返します。
コントローラーこそが、実装していく対象で、リクエストからレスポンスを作る場所なのです。

たったそれだけのことなのです。おさらいしていきましょう。

* リクエストが来たら、常にフロントコントローラーが実行される

* ルーティングシステムは、リクエストの情報とルーティング設定ファイルから、\
  どの PHP コードが実行されるべきか決定する。

* 適切な担当 PHP コードが実行され、そのコードは ``Response`` オブジェクトを作って返す。

アクションにおける Symfony Request
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

あまり深追いはしませんが、一連の流れがどのように処理されていくのか見てみましょう。
Symfony アプリケーションに ``/contact`` ページを追加してみます。
まずは、ルーティングの設定ファイルに ``/contact`` 関連の行を追加します。

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        contact:
            path:     /contact
            defaults: { _controller: AcmeDemoBundle:Main:contact }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="contact" path="/contact">
                <default key="_controller">AcmeDemoBundle:Main:contact</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contact',
        )));

        return $collection;

.. note::

   例ではルーティングコンフィギュレーションの定義に :doc:`YAML </components/yaml/yaml_format>` を使っています。
   XML や PHP で記述することも可能です。

``/contact`` に誰かがアクセスしてきたら、このルートがマッチして、\
指定したコントローラーが実行されます。
詳しくは\ :doc:`ルーティングの章 </book/routing>`\ で説明しますが、\
``AcmeDemoBundle:Main:contact`` という文字列は、\
``MainController`` クラス内の ``contactAction`` メソッドを示す省略記法です。

.. code-block:: php

    // src/Acme/DemoBundle/Controller/MainController.php
    namespace Acme\DemoBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;

    class MainController
    {
        public function contactAction()
        {
            return new Response('<h1>Contact us!</h1>');
        }
    }

この例はとてもシンプルで、"<h1>Contact us!</h1>" という HTML コードから
:class:`Symfony\\Component\\HttpFoundation\\Response` オブジェクトを作っています。
:doc:`コントローラーの章 </book/controller>`\ では、\
コントローラーがどうやってテンプレートをレンダリングするのか、や、\
バラバラのテンプレートファイルから "presentation"  コード(つまり HTML を出力するもの)をどうやって作っているのか、などを説明しています。
ということで、コントローラーでは、データベースとのやりとりや、送信されてきたデータの処理、メール送信といった肝心な部分だけを心配すればいいのです。

Symfony2: ツールではなく、自分のアプリケーションの関心事に取り組むために
------------------------------------------------------------------------

どんなアプリケーションでも、リクエストを解釈して、適切なレスポンスを返すことが目標だということは分かっていただけたと思います。
アプリケーションが大きくなってくると、整理されていてメンテしやすいコードを保つことは難しくなってきます。
それでも、同じように複雑なタスクは矢のように降ってきます。
データベースへの永続化とか、テンプレートのレンダリング・再利用、フォームの処理、\
メール送信、入力のバリデーション、セキュリティの確保など。

とはいえ、別にどの問題もユニークなわけではありません。
Symfony はフレームワークに、アプリケーションを作成するためのツールをたくさん提供しています。
従ってツールを作る必要はありません。
Symfony2 を使えば、面倒な作業は必要ありません。
Symfony フレームワークを使い倒すもよし、一部分だけを使うもよし、です。

.. index::
   single: Symfony2 Components

スタンドアロンなツール: Symfony2 *コンポーネント*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

結局のところ、Symfony2 とはいったい何なのでしょうか？
Symfony2 は、独立した 20 以上のライブラリの集合体で、それらの一つ一つは *どんな* PHP プロジェクトでも使用可能です。
それらを *Symfony2 コンポーネント*\ と呼んでいますが、どんな開発の場合でも、ほとんどすべてのシチュエーションで便利なものとなっています。
いくつか紹介しましょう。

* :doc:`HttpFoundation </components/http_foundation/introduction>` - ``Request`` クラスや ``Response`` クラスの他、\
  セッションやファイルアップロードを扱うクラスもある

* :doc:`Routing </components/routing/introduction>` - 強力で高速なルーティングシステムで、指定されたURL(例: ``/contact``\ )を
  どう処理すべきかという情報にマップする(例:  ``contactAction()`` を実行する)

* `Form`_ - フォーム用のフル機能で柔軟なフレームワークで、フォームの作成や、受付を扱うことができる

* `Validator`_ - データに対するルールづくりのシステムで、ユーザの送信してきたデータが、\
  そのルールに則っているか検証する

* :doc:`ClassLoader </components/class_loader/introduction>` - クラスを使うときに、そのファイルを手動で ``require`` しなくても、\
  ライブラリをオートロードしてくれる

* :doc:`Templating </components/templating/introduction>` - テンプレートのレンダリング、テンプレート継承(レイアウトでテンプレートをデコレートできる)、\
  その他テンプレートの共通タスクなどを扱うツールキット

* `Security`_ - アプリ内のあらゆるタイプのセキュリティ事項を扱う強力なライブラリ

* `Translation`_ アプリ内の文字列を翻訳するためのフレームワーク

すべてのコンポーネントは互いに独立していて、\
Symfony2 フレームワークを使っていようがいまいが、\
*どんな* PHP プロジェクトでも使用できます。
すべて、必要があるときに使えばいいように作られていますし、\
必要であれば置き換えも可能です。


完全なソリューション: Symfony2 *フレームワーク*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

では、Symfony2 *フレームワーク* と言ったときは、一体何を指すのでしょうか。
*Symfony2 フレームワーク* は、PHPライブラリで、次の独立した 2 つのタスクを達成しています。

#. コンポーネント(Symfony2 コンポーネント)とサードパーティ製のライブラリ(メール送信の `Swift Mailer`_ など)を選択して提供すること

#. 実用本位な設定ができ、上記のバラバラになっている部品群を紐付けする"接着剤"としての役割を提供すること

フレームワークの目的は、独立したツール群を統合し、開発者に一貫したエクスペリエンスを提供することです。
フレームワークそれ自体も、Symfony2 バンドル(プラグイン)であり、設定可能です。置き換えることすら可能になっています。

Symfony2 は強力なツール群を提供しており、無理を強いること無く、迅速な Web アプリケーションの開発が可能になっています。
一般的なユーザであれば、実用的なデフォルト値が設定済みのスケルトンプロジェクトが入っている Symfony2 ディストリビューションを使えば、簡単に開発を始めることができます。
上級ユーザであれば、お好きなようにお使いください。

.. _`xkcd`: http://xkcd.com/
.. _`HTTP 1.1 RFC`: http://www.w3.org/Protocols/rfc2616/rfc2616.html
.. _`HTTP Bis`: http://datatracker.ietf.org/wg/httpbis/
.. _`Live HTTP Headers`: https://addons.mozilla.org/en-US/firefox/addon/live-http-headers/
.. _`List of HTTP status codes`: http://en.wikipedia.org/wiki/List_of_HTTP_status_codes
.. _`List of HTTP header fields`: http://en.wikipedia.org/wiki/List_of_HTTP_header_fields
.. _`List of common media types`: http://en.wikipedia.org/wiki/Internet_media_type#List_of_common_media_types
.. _`Form`: https://github.com/symfony/Form
.. _`Validator`: https://github.com/symfony/Validator
.. _`Security`: https://github.com/symfony/Security
.. _`Translation`: https://github.com/symfony/Translation
.. _`Swift Mailer`: http://swiftmailer.org/

.. 2011/07/24 gilbite af374dc7caff75ef88fe2ea5df89a6a7ecd7dd95
.. 2013/12/28 hidenorigoto fb61db9db5e777b8108e5f7aa01aceac8938ffad
