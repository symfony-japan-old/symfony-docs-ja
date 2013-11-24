.. index::
   single: Tests

テスト
======

ソースコードに新しい行を1行追加するたびに、潜在的に新しいバグを追加しているかもしれません。
より優れた、信頼性の高いアプリケーションを構築するには、ユニットテストとファンクショナルテストの両方でコードをテストするべきです。

PHPUnit テスティングフレームワーク
----------------------------------

Symfony2 ではテスティングフレームワークとして、独立したライブラリである PHPUnit を利用します。
PHPUnit についてここでは詳しく解説しませんが、\ `PHPUnitのドキュメント`_\ を読んでおくことをおすすめします。

.. note::

    Symfony2では PHPUnit 3.5.11 以降が必要です。
    Symfony のコアをテストする場合はバージョン 3.6.4 を利用して下さい。

ユニットテストかファンクショナルテストかに関わらず、テストは通常の PHP クラスとして実装します。
標準で、テストはバンドルの `Tests/` ディレクトリ以下に置きます。
この標準に従う限り、全てのテストが以下のコマンドで実行出来ます。

.. code-block:: bash

    # コマンドラインオプションで設定ファイルのあるディレクトリを指定
    $ phpunit -c app/

``-c`` オプションで PHPUnit の設定ファイルのあるディレクトリを指定します。
PHPUnit のオプションについては \ ``app/phpunit.xml.dist`` ファイルを参照して下さい。

.. index::
   single: Tests; Unit Tests

ユニットテスト
--------------

通常、ユニットテストは単一の PHP クラスを対象として記述するテストです。
クラスをまたいだアプリケーション全体の動作のテストについては `Functional Tests`_ を参照して下さい。

Symfony2 におけるユニットテストは、標準的な PHPUnit のテストと同様に記述できます。
仮に、非常にシンプルな ``Calculator`` クラスがバンドルの ``Utility/`` ディレクトリの中にあるとします。

.. code-block:: php

    // src/Acme/DemoBundle/Utility/Calculator.php
    namespace Acme\DemoBundle\Utility;

    class Calculator
    {
        public function add($a, $b)
        {
            return $a + $b;
        }
    }

これをテストするには、\ ``CalculatorTest`` クラスをバンドルの ``Tests/Utility`` ディレクトリに作ります。

.. code-block:: php

    // src/Acme/DemoBundle/Tests/Utility/CalculatorTest.php
    namespace Acme\DemoBundle\Tests\Utility;

    use Acme\DemoBundle\Utility\Calculator;

    class CalculatorTest extends \PHPUnit_Framework_TestCase
    {
        public function testAdd()
        {
            $calc = new Calculator();
            $result = $calc->add(30, 12);

            // assert that your calculator added the numbers correctly!
            $this->assertEquals(42, $result);
        }
    }

.. note::

    規約として、\ ``Tests/`` サブディレクトリはバンドル内の他のディレクトリ構造を模す必要があります。
    したがって、\ ``Utility/`` ディレクトリ内にあるクラスのテストは ``Tests/Utility/`` ディレクトリ以下に配置します。

通常のアプリケーション同様、オートロードは ``app/bootstrap.php.cache`` ファイルによって自動的に有効になります（デフォルトで phpunit.xml.dist ファイルにて設定されています）。

ファイルやディレクトリを指定してテストを実行するには、次のようにします。

.. code-block:: bash

    # Utility ディレクトリ内の全てのテストを実行
    $ phpunit -c app src/Acme/DemoBundle/Tests/Utility/

    # Calculator クラスのテストを実行
    $ phpunit -c app src/Acme/DemoBundle/Tests/Utility/CalculatorTest.php

    # バンドル全体のテストを実行
    $ phpunit -c app src/Acme/DemoBundle/

.. index::
   single: Tests; Functional Tests

ファンクショナルテスト
----------------------

ファンクショナルテストでは、ルーティングからビューまでの、アプリケーションのさまざまなレイヤー間の結合テストを行います。
PHPUnitでのテストの記述としては、ファンクショナルテストはユニットテストと違いはありませんが、ファンクショナルテストでは、次のような特殊なワークフローでテストを行います。

* リクエストの作成
* レスポンスのテスト
* リンクのクリック、またはフォームの送信
* レスポンスのテスト
* クリーンアップと繰り返し

最初のファンクショナルテスト
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ファンクショナルテストはバンドルの ``Tests/Controller`` ディレクトリ以下に置く通常の PHP ファイルです。
例えば ``DemoController`` クラスによって生成されるページのテストを書くには、まず ``WebTestCase`` クラスを継承した ``DemoControllerTest.php`` クラスを作る所から始めます。

Symfony2 Standard Edition が ``DemoController`` 用のシンプルなファンクショナルテストとして提供している `DemoControllerTest`_ を見てみましょう。

.. code-block:: php

    // src/Acme/DemoBundle/Tests/Controller/DemoControllerTest.php
    namespace Acme\DemoBundle\Tests\Controller;

    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;

    class DemoControllerTest extends WebTestCase
    {
        public function testIndex()
        {
            $client = static::createClient();

            $crawler = $client->request('GET', '/demo/hello/Fabien');

            $this->assertGreaterThan(
                0,
                $crawler->filter('html:contains("Hello Fabien")')->count()
            );
        }
    }

.. tip::

    ファンクショナルテスト実行時には、アプリケーションのカーネルを ``WebTestCase`` クラスが準備します。
    これは通常自動で行われますが、カーネルが標準のディレクトリにない場合は、\ ``phpunit.xml.dist`` を修正して ``KERNEL_DIR`` 環境変数にカーネルのディレクトリを設定して下さい。

    .. code-block:: xml

        <phpunit>
            <!-- ... -->
            <php>
                <server name="KERNEL_DIR" value="/path/to/your/app/" />
            </php>
            <!-- ... -->
        </phpunit>

``createClient()`` 静的メソッドは、Web ブラウザのように動作するクライアントを返します。

.. code-block:: php

    $crawler = $client->request('GET', 'hello/Fabien');

``request()`` メソッドは ``Crawler`` オブジェクトを返します。
このオブジェクトを使って、レスポンス内の要素を選択したり、リンクをクリックしたり、フォームを送信したりできます。
（``request()`` メソッドについて詳しくは :ref:`こちら <book-testing-request-method-sidebar>` を参照して下さい）

.. tip::

    Crawler オブジェクトは、レスポンスの内容が XML ドキュメント、または HTML ドキュメントの場合にのみ取得出来ます。
    そうでない場合は ``$client->getResponse()->getContent()`` のようにしてレスポンスの内容を取得します。

リンクをクリックするには、最初にCrawlerオブジェクトでXPath式やCSSセレクタを使ってリンクを選択し、Clientオブジェクトを使ってクリックします。
例えば、以下のコードは ``Greet`` という文字列を含む全てのリンクの中から2番目のものを選択し、クリックします。

.. code-block:: php

    $link = $crawler->filter('a:contains("Greet")')->eq(1)->link();

    $crawler = $client->click($link);

フォームの送信の仕方もほとんど同じです。フォームのボタンを選択し、フォームの値を設定して、送信を実行します。

.. code-block:: php

    $form = $crawler->selectButton('submit')->form();

    // フォームの値を設定
    $form['name'] = 'Lucas';
    $form['form_name[subject]'] = 'Hey there!';

    // フォームを送信
    $crawler = $client->submit($form);

.. tip::

    フォームには、ファイルアップロード機能や、様々なフィールドを扱うためのメソッド（``select()`` や ``tick()`` など）が用意されています。
    詳しくはこの下の\ `フォーム`_\ セクションを参照して下さい。

これでアプリケーションの生成するページを自由に遷移できるようになったので、アサーションを使って意図したとおりに遷移していることを確認しましょう。
Crawler オブジェクトを使って特定の DOM エレメントに対してアサーションを設定するには以下のようにします。

.. code-block:: php

    // レスポンスが指定されたCSSセレクタにマッチすることを検証する
    $this->assertGreaterThan(0, $crawler->filter('h1')->count());

単にあるの文字列がレスポンスのテキスト全体に含まれているかどうか検証する場合や、レスポンスの形式が XML や HTML ではないような場合は、
次のようにレスポンスのテキスト全体に対して検証することも出来ます。

.. code-block:: php

    $this->assertRegExp(
        '/Hello Fabien/',
        $client->getResponse()->getContent()
    );

.. _book-testing-request-method-sidebar:

.. sidebar:: ``request()`` メソッドの詳細:

    ``request()`` メソッドのシグネチャは以下のとおりです。

    .. code-block:: php

        request(
            $method,
            $uri,
            array $parameters = array(),
            array $files = array(),
            array $server = array(),
            $content = null,
            $changeHistory = true
        )

    ``server`` 配列は PHP の `$_SERVER`_ スーパーグローバル変数に相当します。
    例えばリクエストの HTTP ヘッダに ``Content-Type``, ``Referer``, ``X-Requested-With``
    を渡すには以下のようにします（非標準のヘッダ名には ``HTTP_`` プレフィクスを付けることに注意して下さい）。

    .. code-block:: php

        $client->request(
            'GET',
            '/demo/hello/Fabien',
            array(),
            array(),
            array(
                'CONTENT_TYPE'          => 'application/json',
                'HTTP_REFERER'          => '/foo/bar',
                'HTTP_X-Requested-With' => 'XMLHttpRequest',
            )
        );

.. index::
   single: Tests; Assertions

.. sidebar:: 便利なアサーションメソッド

    よく使われるアサーションメソッドの一覧です。

    .. code-block:: php

        use Symfony\Component\HttpFoundation\Response;

        // ...

        // subtitle クラスを持つ h2 タグが1つ以上あることを検証
        $this->assertGreaterThan(
            0,
            $crawler->filter('h2.subtitle')->count()
        );

        // ページ内に h2 タグがちょうど4つあることを検証
        $this->assertCount(4, $crawler->filter('h2'));

        // "Content-Type" ヘッダが "application/json" であることを検証
        $this->assertTrue(
            $client->getResponse()->headers->contains(
                'Content-Type',
                'application/json'
            )
        );

        // レスポンスの内容が正規表現にマッチすることを検証
        $this->assertRegExp('/foo/', $client->getResponse()->getContent());

        // レスポンスのステータスコードが 2xx であることを検証
        $this->assertTrue($client->getResponse()->isSuccessful());
        // レスポンスのステータスコードが 404 であることを検証
        $this->assertTrue($client->getResponse()->isNotFound());
        // レスポンスのステータスコードが 200 であることを検証
        $this->assertEquals(
            Response::HTTP_OK,
            $client->getResponse()->getStatusCode()
        );

        // レスポンスが /demo/contact へのリダイレクトであることを検証
        $this->assertTrue(
            $client->getResponse()->isRedirect('/demo/contact')
        );
        // レスポンスがどこかへのリダイレクトであることを検証
        $this->assertTrue($client->getResponse()->isRedirect());

    .. versionadded:: 2.4
        HTTP ステータスコードの検証は Symfony 2.4 で追加されました。

.. index::
   single: Tests; Client

テストクライアント
------------------

テスト用の Client オブジェクトは、Web ブラウザのような HTTP クライアントをシミュレートし、Symfony2 アプリケーションに対してリクエストを送信します。

.. note::

    Clientオブジェクトは、\ ``BrowserKit``\ コンポーネントと\ ``Crawler``\ コンポーネントを利用しています。

リクエストの送信
~~~~~~~~~~~~~~~~

クライアントから Symfony2 アプリケーションへリクエストを送信するには、次のようにします。

.. code-block:: php

    $crawler = $client->request('GET', '/hello/Fabien');

``request()``\ メソッドは、引数としてHTTPメソッドとURLをとり、\ ``Crawler``\ インスタンスを返します。

レスポンスからDOM要素を探すには Crawler オブジェクトを使います。見つかった要素を使って、リンクのクリックやフォームの送信を行えます。

.. code-block:: php

    $link = $crawler->selectLink('Go elsewhere...')->link();
    $crawler = $client->click($link);

    $form = $crawler->selectButton('validate')->form();
    $crawler = $client->submit($form, array('name' => 'Fabien'));

``click()`` メソッドや ``submit()`` メソッドは ``Crawler`` オブジェクトを返します。
これらのメソッドはアプリケーションをブラウズする最適な方法です。
フォームの HTTP メソッドを調べたり、ファイルアップロードの API を利用できたりと、様々な機能を提供してくれます。

.. tip::

    ``Link`` オブジェクトと ``Form`` オブジェクトの詳細については、\ `Crawlerオブジェクト`_\ の節を参照してください。

``response()`` メソッドで、フォームの送信などのより複雑な操作をすることも出来ます。

.. code-block:: php

    // フォームの送信
    $client->request('POST', '/submit', array('name' => 'Fabien'));

    // ファイルアップロードのあるフォームの送信
    $client->request(
        'POST',
        '/submit',
        array(),
        array(),
        array('CONTENT_TYPE' => 'application/json'),
        '{"name":"Fabien"}'
    );

    // ファイルアップロード
    use Symfony\Component\HttpFoundation\File\UploadedFile;

    $photo = new UploadedFile(
        '/path/to/photo.jpg',
        'photo.jpg',
        'image/jpeg',
        123
    );
    $client->request(
        'POST',
        '/submit',
        array('name' => 'Fabien'),
        array('photo' => $photo)
    );

    // HTTP ヘッダを指定して DELETE リクエストを送信
    $client->request(
        'DELETE',
        '/post/12',
        array(),
        array(),
        array('PHP_AUTH_USER' => 'username', 'PHP_AUTH_PW' => 'pa$$word')
    );

また、各リクエストを独立したPHPプロセスで実行することで、同一のスクリプト内で複数のクライアントを実行した場合の副作用を回避できます。

.. code-block:: php

    $client->insulate();

ブラウジング
~~~~~~~~~~~~

Clientオブジェクトは、実際のWebブラウザで実行可能なさまざまな操作をサポートしています。

.. code-block:: php

    $client->back();
    $client->forward();
    $client->reload();

    // すべてのCookieと履歴を削除
    $client->restart();

内部オブジェクトへのアクセス
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    ``getInternalRequest()``, ``getInternalResponse()`` メソッドは Symfony 2.3 で追加されました。

Clientオブジェクトを使ってアプリケーションのテストを記述する際に、Clientの内部オブジェクトにアクセスしたい場合があるかもしれません。

.. code-block:: php

    $history   = $client->getHistory();
    $cookieJar = $client->getCookieJar();

直前のリクエストに関する、次のようなオブジェクトも取得できます。

.. code-block:: php

    // HttpKernel のリクエストインスタンスを取得
    $request  = $client->getRequest();

    // BrowserKit のリクエストインスタンスを取得
    $request  = $client->getInternalRequest();

    // HttpKernel のレスポンスインスタンスを取得
    $response = $client->getResponse();

    // BrowserKit のレスポンスインスタンスを取得
    $response = $client->getInternalResponse();

    $crawler  = $client->getCrawler();

リクエストを独立したプロセスで実行していない場合は、\ ``Container``\ オブジェクトや\ ``Kernel``\ オブジェクトにもアクセスできます。

.. code-block:: php

    $container = $client->getContainer();
    $kernel    = $client->getKernel();

Containerオブジェクトへのアクセス
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ファンクショナルテストでは、レスポンスのみをテストすることが推奨されています。しかし、アサーションを記述するために内部オブジェクトにアクセスしたい状況もあるでしょう。このような場合は、次のように Dependency Injection コンテナにアクセスします。

.. code-block:: php

    $container = $client->getContainer();

クライアントを独立したPHPプロセスで実行している場合や、HTTPレイヤーを使っている場合は、上のコードで Dependency Injection コンテナを取得することはできない点に注意してください。
アプリケーションで利用可能なサービスの一覧は ``app/console container:debug`` で参照出来ます。

.. tip::

    チェックしたい情報をプロファイラから取得できる場合は、 Dependency Injection コンテナの代わりにプロファイラを使ってください。

プロファイリングデータの取得
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

リクエストのプロファイラを有効にすれば、リクエストの内部処理の情報を取得することが出来ます。
プロファイラを利用すれば、あるページのリクエスト中に実行される DB リクエストが一定回数以下であるかどうかなどを確認出来ます。

プロファイラは以下のようにして取得できます。

.. code-block:: php

    // 次に実行するリクエストのプロファイラを有効にする
    $client->enableProfiler();

    $crawler = $client->request('GET', '/profiler');

    // プロファイラを取得
    $profile = $client->getProfile();

プロファイラの詳細については、クックブックの\ :doc:`/cookbook/testing/profiling`\ を参照して下さい。

リダイレクト
~~~~~~~~~~~~

リクエストの結果がリダイレクトだった場合でも、クライアントは自動ではリダイレクト先へ遷移しません。
``followRedirect()`` メソッドで明示的に遷移させる必要があります。

.. code-block:: php

    $crawler = $client->followRedirect();

全てのリダイレクトに対して自動的に遷移させたい場合は ``followRedirects()`` メソッドを使用します。

.. code-block:: php

    $client->followRedirects();

.. index::
   single: Tests; Crawler

Crawlerオブジェクト
-------------------

Clientオブジェクトからリクエストを送信すると、Crawlerインスタンスが返されます。
このCrawlerを使って、HTMLドキュメントを走査し、ノードを選択し、リンクやフォームを検索します。

DOM の走査
~~~~~~~~~~

Crawlerには、jQueryに似た、HTML/XMLドキュメントのDOMを走査するメソッドがあります。
例えば以下のようにすると、\ ``input[type=submit]`` にマッチするエレメントを検索し、そのうち最後の要素を選択し、さらにその直近の親エレメントを取得します。

.. code-block:: php

    $newCrawler = $crawler->filter('input[type=submit]')
        ->last()
        ->parents()
        ->first()
    ;

他にもたくさんのメソッドがあります。

+------------------------+----------------------------------------------------+
| Method                 | Description                                        |
+========================+====================================================+
| ``filter('h1.title')`` | CSSセレクタにマッチするノード                      |
+------------------------+----------------------------------------------------+
| ``filterXpath('h1')``  | XPath式にマッチするノード                          |
+------------------------+----------------------------------------------------+
| ``eq(1)``              | 指定したインデックスのノード                       |
+------------------------+----------------------------------------------------+
| ``first()``            | 最初のノード                                       |
+------------------------+----------------------------------------------------+
| ``last()``             | 最後のノード                                       |
+------------------------+----------------------------------------------------+
| ``siblings()``         | 兄弟のノード                                       |
+------------------------+----------------------------------------------------+
| ``nextAll()``          | 後の兄弟ノード                                     |
+------------------------+----------------------------------------------------+
| ``previousAll()``      | 前の兄弟ノード                                     |
+------------------------+----------------------------------------------------+
| ``parents()``          | 親ノード、先祖ノード                               |
+------------------------+----------------------------------------------------+
| ``children()``         | 子ノード                                           |
+------------------------+----------------------------------------------------+
| ``reduce($lambda)``    | callableがfalseを返さないノード                    |
+------------------------+----------------------------------------------------+

各メソッドは条件にマッチした新しいCrawlerオブジェクトを返すので、チェインさせていくことで、インタラクティブにノードを絞り込んでいくことができます。

.. code-block:: php

    $crawler
        ->filter('h1')
        ->reduce(function ($node, $i)
        {
            if (!$node->getAttribute('class')) {
                return false;
            }
        })
        ->first();

.. tip::

    ``count()`` 関数を使って、現在のCrawlerオブジェクトが保持しているノードの数を取得できます:
    ``count($crawler)``

情報の抽出
~~~~~~~~~~

Crawler からノードの情報を抽出できます。

.. code-block:: php

    // 最初のノードの、指定した属性の値を返す
    $crawler->attr('class');

    // 最初のノードの値を返す
    $crawler->text();

    // すべてのノードから、配列で指定した属性の値を抽出する（_textはノードの値を返す）
    $crawler->extract(array('_text', 'href'));

    // 各ノードに対してラムダを実行し、結果を配列として返す
    $data = $crawler->each(function ($node, $i)
    {
        return $node->getAttribute('href');
    });

リンク
~~~~~~

リンクの選択は上述の走査メソッドでも可能ですが、\ ``selectLink()`` メソッドを使うとより簡単です。

.. code-block:: php

    $crawler->selectLink('Click here');

これで、指定された文字列を含むテキストリンク、または alt 属性に指定された文字列を含むリンク付き画像が選択出来ます。
他の走査メソッド同様、これも ``Crawler`` オブジェクトを返します。

リンクの ``Crawler`` オブジェクトからは ``Link`` オブジェクトを取得できます。
``Link`` オブジェクトを使って ``getMethod()``\ 、 ``getUri()`` などの便利なメソッドを利用することが出来ます。
リンクをクリックするには、クライアントの ``click()`` メソッドに取得した ``Link`` オブジェクトを渡します。

.. code-block:: php

    $link = $crawler->selectLink('Click here')->link();

    $client->click($link);

.. tip::

    ``links()``\ メソッドは、すべてのノードの\ ``Link``\  オブジェクトの配列を返します。

フォーム
~~~~~~~~

リンクと同様、\ ``selectButton()``\ メソッドを使ってフォームを選択できます。

.. code-block:: php

    $buttonCrawlerNode = $crawler->selectButton('submit');

.. note::

    この処理では、フォーム自体ではなく、フォームのボタンを選択していることに注意してください。フォームには複数のボタンが存在する可能性があります。走査APIを使う際に、単一のボタンを特定する必要があることを覚えておいてください。

``selectButton()`` メソッドで ``button``  タグを選択し、 ``input`` タグの内容を送信します。
ボタンを選択するために利用できる値がいくつかあります。

* ``value``\ 属性の値

* 画像の\ ``id``\ または\ ``alt``\ 属性の値

* ``button``\ タグの\ ``id``\ または\ ``name``\ 属性の値

ボタンに対応するノードが見つかったら、 ``form()`` メソッドでボタンノードを囲んでいる ``Form`` インスタンスを取得できます。

.. code-block:: php

    $form = $buttonCrawlerNode->form();

``form()``\ メソッドを呼び出す際に、フィールドの値を配列として渡すことで、フォームのデフォルト値を上書きできます。

.. code-block:: php

    $form = $buttonCrawlerNode->form(array(
        'name'              => 'Fabien',
        'my_form[subject]'  => 'Symfony rocks!',
    ));

また、フォームで特定のHTTPメソッドをシミュレートしたい場合は、2つ目の引数に指定します。

.. code-block:: php

    $form = $buttonCrawlerNode->form(array(), 'DELETE');

Clientから\ ``Form``\ インスタンスを送信します。

.. code-block:: php

    $client->submit($form);

フィールドの値は ``submit()`` メソッドの2つ目の引数で渡すこともできます。

.. code-block:: php

    $client->submit($form, array(
        'name'              => 'Fabien',
        'my_form[subject]'  => 'Symfony rocks!',
    ));

さらに複雑な状況の場合は、\ ``Form``\ インスタンスを配列のようにアクセスして、各フィールドの値を個別に設定できます。

.. code-block:: php

    // フィールドの値を変更
    $form['name'] = 'Fabien';
    $form['my_form[subject]'] = 'Symfony rocks!';

フィールドのタイプごとに、値を操作する便利なAPIが用意されています。

.. code-block:: php

    // radioのオプションを選択
    $form['country']->select('France');

    // checkboxをチェック
    $form['like_symfony']->tick();

    // ファイルをアプロード
    $form['photo']->upload('/path/to/lucas.jpg');

.. .. tip::
..     "invalid" な select や radio の値を取得する方法については
..     :ref:`components-dom-crawler-invalid` を参照して下さい。
..     ↑リンク先未翻訳につきコメントアウト

.. tip::

    フォームに送信される値は ``Form`` オブジェクトの ``getValues()`` メソッドで取得できます。
    アップロードされたファイルにアクセスするには、\ ``getFiles()``\ メソッドの戻り値の配列を使います。
    ``getPhpValues()`` と ``getPhpFiles()`` は、送信された値をPHPフォーマットで返します（各括弧記法のキーをPHPの配列へ変換します）。

.. index::
   pair: Tests; Configuration

テストの設定
------------

ファンクショナルテストで紹介した ``Client`` オブジェクトは特別な ``test`` 環境でカーネルを生成します。
``test`` 環境では Symfony は ``app/config/config_test.yml`` をロードするため、テスト専用の設定に調整することが出来ます。

例えば、Swift Mailer はデフォルトで ``test`` 環境では実際にメールを送信しないように設定されています。
これは ``swiftmailer`` 設定オプションで確認出来ます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml

        # ...
        swiftmailer:
            disable_delivery: true

    .. code-block:: xml

        <!-- app/config/config_test.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd">

            <!-- ... -->
            <swiftmailer:config disable-delivery="true" />
        </container>

    .. code-block:: php

        // app/config/config_test.php

        // ...
        $container->loadFromExtension('swiftmailer', array(
            'disable_delivery' => true,
        ));

``createClient()`` メソッドにオプションを渡すことで、デフォルトの環境 (``test``) やデバッグモードの値 (``true``) を変更できます。

.. code-block:: php

    $client = static::createClient(array(
        'environment' => 'my_test_env',
        'debug'       => false,
    ));

アプリケーションの動作がHTTPヘッダーに依存している場合、\ ``createClient()``\ メソッドの第2引数として渡すことが出来ます。

.. code-block:: php

    $client = static::createClient(array(), array(
        'HTTP_HOST'       => 'en.example.com',
        'HTTP_USER_AGENT' => 'MySuperBrowser/1.0',
    ));

リクエストごとにHTTPヘッダーの値を変更することもできます。

.. code-block:: php

    $client->request('GET', '/', array(), array(), array(
        'HTTP_HOST'       => 'en.example.com',
        'HTTP_USER_AGENT' => 'MySuperBrowser/1.0',
    ));

.. .. tip::
..     The test client is available as a service in the container in the ``test``
..     environment (or wherever the :ref:`framework.test <reference-framework-test>`
..     option is enabled). This means you can override the service entirely
..     if you need to.
..     よく分からないしリンク先も未翻訳なのでコメントアウト

.. index::
   pair: PHPUnit; Configuration

PHPUnitの設定
~~~~~~~~~~~~~

アプリケーションの PHPUnit の設定は ``phpunit.xml.dist`` に記述されています。
このファイルを直接編集するか、\ ``phpunit.xml`` ファイルを作ってローカルマシン用に設定をカスタマイズ出来ます。

.. tip::

    バージョン管理システムのリポジトリには ``phpunit.xml.dist`` ファイルのみ保存し、\ ``phpunit.xml`` ファイルは無視するよう設定してください。

デフォルトでは ``phpunit`` コマンドは、標準的なディレクトリ構成のバンドル内のテスト（\ ``src/*/Bundle/Tests/`` または ``src/*/Bundle/*Bundle/Tests/`` ディレクトリ以下のテスト）だけ実行します。
テストのディレクトリを追加するのは簡単です。
例えば次のように設定すると、サードパーティのバンドルにあるテストが追加されます。

.. code-block:: xml

    <!-- hello/phpunit.xml.dist -->
    <testsuites>
        <testsuite name="Project Test Suite">
            <directory>../src/*/*Bundle/Tests</directory>
            <directory>../src/Acme/Bundle/*Bundle/Tests</directory>
        </testsuite>
    </testsuites>

コードカバレッジに別のディレクトリを追加するには、\ ``<filter>``\ セクションも併せて編集してください。

.. code-block:: xml

    <!-- ... -->
    <filter>
        <whitelist>
            <directory>../src</directory>
            <exclude>
                <directory>../src/*/*Bundle/Resources</directory>
                <directory>../src/*/*Bundle/Tests</directory>
                <directory>../src/Acme/Bundle/*Bundle/Resources</directory>
                <directory>../src/Acme/Bundle/*Bundle/Tests</directory>
            </exclude>
        </whitelist>
    </filter>

Cookbookの参考記事
------------------

* :doc:`/components/dom_crawler`
* :doc:`/components/css_selector`
* :doc:`/cookbook/testing/http_authentication`
* :doc:`/cookbook/testing/insulating_clients`
* :doc:`/cookbook/testing/profiling`
* :doc:`/cookbook/testing/bootstrap`

.. _`DemoControllerTest`: https://github.com/symfony/symfony-standard/blob/master/src/Acme/DemoBundle/Tests/Controller/DemoControllerTest.php
.. _`$_SERVER`: http://php.net/manual/ja/reserved.variables.server.php
.. _PHPUnitのドキュメント: http://www.phpunit.de/manual/3.8/ja/

.. 2013/11/24 monmonmon 9df6556e294c2fa9548f93083529e7a9ad9d6ea7
.. 2011/07/23 gilbite 9df6556e294c2fa9548f93083529e7a9ad9d6ea7
.. 2011/03/01 hidenorigoto unknown
