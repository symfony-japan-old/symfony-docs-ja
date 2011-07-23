.. 2011/07/23 gilbite 9df6556e294c2fa9548f93083529e7a9ad9d6ea7
.. 2011/03/01 hidenorigoto unknown

.. index::
   single: Tests

テスト
======

ソースコードに新しい行を1行追加するたびに、潜在的に新しいバグを追加しているかもしれません。
テストを自動化しておくことで、このようなバグを回避できます。
ここでは、Symfony2アプリケーション向けにユニットテストとファンクショナルテストを記述する方法について説明します。

テスティングフレームワーク
--------------------------

Symfony2のテストは、PHPUnitと、PHPUnitで培われてきたベストプラクティスやいくつかの規約に大きく依存しています。
これらについては詳しく解説しませんが、まだ読まれていない場合は、\ `PHPUnitのドキュメント`_\ を読んでおくことをおすすめします。

.. note::

    Symfony2では、PHPUnit 3.5.11以降が必要です。

デフォルトのPHPUnitの設定では、バンドルの\ ``Tests/``\ ディレクトリ以下にあるテストが実行されます。

.. code-block:: xml

    <!-- app/phpunit.xml.dist -->

    <phpunit bootstrap="../src/autoload.php">
        <testsuites>
            <testsuite name="Project Test Suite">
                <directory>../src/*/*Bundle/Tests</directory>
            </testsuite>
        </testsuites>

        ...
    </phpunit>

指定したアプリケーションのテストスイートを実行するには、次のようにします。

.. code-block:: bash

    # コマンドラインで設定のあるディレクトリを指定する
    $ phpunit -c app/

    # または、アプリケーションディレクトリからphpunitコマンドを実行する
    $ cd app/
    $ phpunit

.. tip::

    ``--coverage-html``\ オプションを指定すると、コードカバレッジが生成されます。

.. index::
   single: Tests; Unit Tests

ユニットテスト
--------------

Symfony2におけるユニットテストの記述は、標準的なPHPUnitのテストの記述と同じです。
規約として、バンドル内のディレクトリ構造を\ ``Tests/``\ サブディレクトリ以下にも同じように作り、そこへテストコードを格納してください。
したがって、\ ``Acme\HelloBundle\Model\Article``\ クラスのテストは、\ ``Acme/HelloBundle/Tests/Model/ArticleTest.php``\ ファイルに記述します。

ユニットテストでは、\ ``src/autoload.php``\ ファイルに記述したオートロードが自動的に有効になります（デフォルトで\ ``phpunit.xml.dist``\ ファイルにて設定されています）。

ファイルやディレクトリを指定してテストを実行するには、次のようにします。

.. code-block:: bash

    # Controllerディレクトリのすべてのテストを実行
    $ phpunit -c app src/Acme/HelloBundle/Tests/Controller/

    # Modelディレクトリのすべてのテストを実行
    $ phpunit -c app src/Acme/HelloBundle/Tests/Model/

    # Articleクラスのテストを実行
    $ phpunit -c app src/Acme/HelloBundle/Tests/Model/ArticleTest.php

    # Bundleディレクトリ以下のすべてのテストを実行
    $ phpunit -c app src/Acme/HelloBundle/

.. index::
   single: Tests; Functional Tests

ファンクショナルテスト
----------------------

ファンクショナルテストでは、ルーティングからビューまでの、アプリケーションのさまさまなレイヤー間の結合テストを行います。
PHPUnitでのテストの記述としては、ファンクショナルテストはユニットテストと違いはありませんが、ファンクショナルテストでは、次のような特殊なワークフローでテストを行います。

* リクエストの作成
* レスポンスのテスト
* リンクのクリック、またはフォームの送信
* レスポンスのテスト
* クリーンアップと繰り返し

リクエストの送信、クリック、フォームの送信は、アプリケーションと対話可能なクライアントによって実行されます。
このクライアントを使うには、Symfony2の\ ``WebTestCase``\ クラスを継承したテストクラスを使います。
Symfony2 Standard Editionには、\ ``DemoController``\ 用のシンプルなファンクショナルテストがあり、次のようなコードになっています。

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

            $this->assertTrue($crawler->filter('html:contains("Hello Fabien")')->count() > 0);
        }
    }

``createClient()``\ メソッドは、現在のアプリケーションに関連付けられたクライアントを返します。

.. code-block:: php

    $crawler = $client->request('GET', 'hello/Fabien');

``request()``\ メソッドは\ ``Crawler``\ オブジェクトを返します。
このオブジェクトを使って、Response内の要素を選択したり、リンクをクリックしたり、フォームを送信したりできます。

.. tip::

    Crawlerオブジェクトは、Responseの内容がXMLドキュメント、またはHTMLドキュメントの場合に使えます。
    他の形式の場合は、\ ``$client->getResponse()->getContent()``\ のようにしてResponseの内容を取得します。

リンクをクリックするには、最初にCrawlerオブジェクトでXPath式やCSSセレクタを使ってリンクを選択し、Clientオブジェクトを使ってクリックします。

.. code-block:: php

    $link = $crawler->filter('a:contains("Greet")')->eq(1)->link();

    $crawler = $client->click($link);

フォームの送信もほとんど同じで、フォームのボタンを選択し、いくつかのフォームの値を設定して対応するフォームを送信します。

.. code-block:: php

    $form = $crawler->selectButton('submit')->form();

    // フォームの値を設定
    $form['name'] = 'Lucas';

    // フォームを送信
    $crawler = $client->submit($form);

``Form``\ の各フィールドでは、フィールドのタイプに応じた特殊なメソッドを使えます。

.. code-block:: php

    // inputフィールドに値を設定
    $form['name'] = 'Lucas';

    // オプションの選択や、ラジオボタンの選択
    $form['country']->select('France');

    // checkboxフィールドをチェック
    $form['like_symfony']->tick();

    // ファイルをアップロード
    $form['photo']->upload('/path/to/lucas.jpg');

一度に1つずつフィールドを設定するのではなく、\ ``submit()``\ メソッドに配列形式で値を渡すこともできます。

.. code-block:: php

    $crawler = $client->submit($form, array(
        'name'         => 'Lucas',
        'country'      => 'France',
        'like_symfony' => true,
        'photo'        => '/path/to/lucas.jpg',
    ));

これらの機能を使ってアプリケーション内の画面遷移を実行し、アサーションを使って意図したとおりに遷移していることを確認できます。
次のように、Crawlerオブジェクトを使って特定のDOMに対してアサーションを設定します。

.. code-block:: php

    // レスポンスが指定されたCSSセレクタにマッチすることを検証する
    $this->assertTrue($crawler->filter('h1')->count() > 0);

Responseの内容に特定のテキストが含まれていることを検証したり、Responseの形式がXMLやHTMLではない場合は、次のようにResponseの内容を直接検証します。

.. code-block:: php

    $this->assertRegExp('/Hello Fabien/', $client->getResponse()->getContent());

.. index::
   single: Tests; Assertions

便利なアサーション
~~~~~~~~~~~~~~~~~~

テストを記述していると、似たようなアサーションを何度も記述していることに気づくでしょう。より早くテストを記述するために、よく利用される便利なアサーションを紹介します。

::

    // 指定したCSSセレクタにレスポンスがマッチすることを検証する
    $this->assertTrue($crawler->filter($selector)->count() > 0);

    // 指定されたCSSセレクタにレスポンスがn回マッチすることを検証する
    $this->assertEquals($count, $crawler->filter($selector)->count());

    // レスポンスヘッダーに特定の値があることを検証する
    $this->assertTrue($client->getResponse()->headers->contains($key, $value));

    // レスポンスの内容が正規表現にマッチすることを検証する
    $this->assertRegExp($regexp, $client->getResponse()->getContent());

    // レスポンスのステータスコードを検証する
    $this->assertTrue($client->getResponse()->isSuccessful());
    $this->assertTrue($client->getResponse()->isNotFound());
    $this->assertEquals(200, $client->getResponse()->getStatusCode());

    // レスポンスのステータスコードがリダイレクトであることを検証する
    $this->assertTrue($client->getResponse()->isRedirected('google.com'));

.. _PHPUnitのドキュメント: http://www.phpunit.de/manual/3.5/ja/

.. index::
   single: Tests; Client

テストクライアント
------------------

テスト用のClientオブジェクトは、WebブラウザのようなHTTPクライアントをシミュレートします。

.. note::

    Clientオブジェクトは、\ ``BrowserKit``\ コンポーネントと\ ``Crawler``\ コンポーネントを利用しています。

リクエストの送信
~~~~~~~~~~~~~~~~

クライアントから、Symfony2アプリケーションへリクエストを送信するには、次のようにします。

.. code-block:: php

    $crawler = $client->request('GET', '/hello/Fabien');

``request()``\ メソッドは、引数としてHTTPメソッドとURLをとり、\ ``Crawler``\ インスタンスを返します。

ResponseからDOM要素を探すには、Crawlerオブジェクトを使います。見つかった要素を使って、リンクのクリックやフォームの送信を行えます。

.. code-block:: php

    $link = $crawler->selectLink('Go elsewhere...')->link();
    $crawler = $client->click($link);

    $form = $crawler->selectButton('validate')->form();
    $crawler = $client->submit($form, array('name' => 'Fabien'));

``click()``\ メソッドと\ ``submit()``\ メソッドは、\ ``Crawler``\ オブジェクトを返します。
これらのメソッドにより詳細な部分を隠蔽できるので、効率よくアプリケーションの遷移を記述できます。たとえば、フォームを送信する場合はHTTPメソッドとフォームのURLが自動的に検出され、ファイルを手軽にアップロードするAPIもあります。フォームに送信された値は、デフォルト値とマージされるといった機能もあります。

.. tip::

    ``Link``\ オブジェクトと\ ``Form``\ オブジェクトの詳細については、Crawlerの節を参照してください。

フォームの送信や複雑なリクエストをシミュレートする別の方法として、\ ``request()``\ メソッドに追加の引数を指定することもできます。

.. code-block:: php

    // フォームの送信
    $client->request('POST', '/submit', array('name' => 'Fabien'));

    // ファイルアップロードのあるフォームの送信
    $client->request('POST', '/submit', array('name' => 'Fabien'), array('photo' => '/path/to/photo'));

    // HTTPヘッダーを指定
    $client->request('DELETE', '/post/12', array(), array(), array('PHP_AUTH_USER' => 'username', 'PHP_AUTH_PW' => 'pa$$word'));

リクエストからリダイレクトのレスポンスが返された場合は、クライアントは自動的にリダイレクト先へ遷移します。
この動作は、\ ``followRedirects()``\ メソッドで変更できます。

.. code-block:: php

    $client->followRedirects(false);

クライアントをリダイレクト先へ遷移しないようにした場合でも、\ ``followRedirect()``\ メソッドを使って強制的にリダイレクトさせることができます。

.. code-block:: php

    $crawler = $client->followRedirect();

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

Clientオブジェクトを使ってアプリケーションのテストを記述する際に、Clientの内部オブジェクトにアクセスしたい場合があるかもしれません。

.. code-block:: php

    $history   = $client->getHistory();
    $cookieJar = $client->getCookieJar();

直前のリクエストに関連付けられた、次のようなオブジェクトも取得できます。

.. code-block:: php

    $request  = $client->getRequest();
    $response = $client->getResponse();
    $crawler  = $client->getCrawler();

リクエストを独立したプロセスで実行していない場合は、\ ``Container``\ オブジェクトや\ ``Kernel``\ オブジェクトにもアクセスできます。

.. code-block:: php

    $container = $client->getContainer();
    $kernel    = $client->getKernel();

Containerオブジェクトへのアクセス
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ファンクショナルテストでは、Responseのみをテストすることが推奨されています。しかし、アサーションを記述するために内部オブジェクトにアクセスしたい状況もあるでしょう。このような場合は、次のようにDIコンテナにアクセスします。

.. code-block:: php

    $container = $client->getContainer();

クライアントを独立したPHPプロセスで実行している場合や、HTTPレイヤーを使っている場合は、上のコードでDIコンテナを取得することはできない点に注意してください。

.. tip::

    チェックしたい情報をプロファイラから取得できる場合は、DIコンテナの代わりにプロファイラを使ってください。

Profilerデータへのアクセス
~~~~~~~~~~~~~~~~~~~~~~~~~~~

プロファイラによって集められたデータを検証する場合は、次のようにして現在のリクエストに対するプロファイラを取得できます。

.. code-block:: php

    $profile = $client->getProfile();

リダイレクト
~~~~~~~~~~~~

Clientオブジェクトは、デフォルトではHTTPリダイレクトによる自動遷移に従わず、
リダイレクトされる前のResponseオブジェクトを取得・検証することができます。
自動的にリダイレクトさせたい場合は\ ``followRedirect()``\ メソッドを呼び出します。　

::

    // リダイレクトが発生するような処理(フォームの送信など)

    // リダイレクトに従う
    $crawler = $client->followRedirect();



Clientオブジェクトが常に自動的にリダイレクトするようにするには、
\ ``followRedirects()``\ メソッドを使用します。

::

    $client->followRedirects();

    $crawler = $client->request('GET', '/');

    // リダイレクトは全て自動遷移

    // 手動でリダイレクト先へ遷移するように戻す
    $client->followRedirects(false);

.. index::
   single: Tests; Crawler

Crawlerオブジェクト
-------------------

Clientオブジェクトからリクエストを送信すると、Crawlerインスタンスが返されます。
このCrawlerを使って、HTMLドキュメントを走査し、ノードを選択し、リンクやフォームを検索します。

Crawlerインスタンスの作成
~~~~~~~~~~~~~~~~~~~~~~~~~

Clientオブジェクトを使ってリクエストを送信すると、Crawlerインスタンスが自動的に作られますが、手作業でCrawlerオブジェクトを作ることもできます。

.. code-block:: php

    use Symfony\Component\DomCrawler\Crawler;

    $crawler = new Crawler($html, $url);

Crawlerのコンストラクタは2つの引数をとります。2つめの引数は、リンクやフォームの絶対URLの生成に使われます。1つめの引数には、次のうちの1つを渡します。

* HTMLドキュメント
* XMLドキュメント
* ``DOMDocument``\ インスタンス
* ``DOMNodeList``\ インスタンス
* ``DOMNode``\ インスタンス
* 上記を要素とする配列

Crawlerインスタンスを作成後、次のようなノードを追加できます。

+-----------------------+----------------------------------+
| メソッド              | 説明                             |
+=======================+==================================+
| ``addHTMLDocument()`` | HTMLドキュメント                 |
+-----------------------+----------------------------------+
| ``addXMLDocument()``  | XMLドキュメント                  |
+-----------------------+----------------------------------+
| ``addDOMDocument()``  | ``DOMDocument``\ インスタンス    |
+-----------------------+----------------------------------+
| ``addDOMNodeList()``  | ``DOMNodeList``\ インスタンス    |
+-----------------------+----------------------------------+
| ``addDOMNode()``      | ``DOMNode``\ インスタンス        |
+-----------------------+----------------------------------+
| ``addNodes()``        | 上記を要素とする配列             |
+-----------------------+----------------------------------+
| ``add()``             | 上記の要素のどれでも指定可能     |
+-----------------------+----------------------------------+

走査
~~~~

Crawlerには、jQueryに似た、HTML/XMLドキュメントのDOMを走査するメソッドがあります。

+-----------------------+----------------------------------------------------+
| メソッド              | 説明                                               |
+=======================+====================================================+
| ``filter('h1')``      | CSSセレクタにマッチするノード                      |
+-----------------------+----------------------------------------------------+
| ``filterXpath('h1')`` | XPath式にマッチするノード                          |
+-----------------------+----------------------------------------------------+
| ``eq(1)``             | 指定したインデックスのノード                       |
+-----------------------+----------------------------------------------------+
| ``first()``           | 最初のノード                                       |
+-----------------------+----------------------------------------------------+
| ``last()``            | 最後のノード                                       |
+-----------------------+----------------------------------------------------+
| ``siblings()``        | 兄弟のノード                                       |
+-----------------------+----------------------------------------------------+
| ``nextAll()``         | 後の兄弟ノード                                     |
+-----------------------+----------------------------------------------------+
| ``previousAll()``     | 前の兄弟ノード                                     |
+-----------------------+----------------------------------------------------+
| ``parents()``         | 親ノード                                           |
+-----------------------+----------------------------------------------------+
| ``children()``        | 子ノード                                           |
+-----------------------+----------------------------------------------------+
| ``reduce($lambda)``   | callableがfalseを返さないノード                    |
+-----------------------+----------------------------------------------------+

各メソッドは、希望する条件にマッチした新しいCrawlerオブジェクトを返すので、チェインさせていくことで、インタラクティブにノードを絞り込んでいくことができます。

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

Crawlerを使って、ノードから情報を抽出できます。

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

走査メソッドを使ってリンクを選択できますが、\ ``selectLink()``\ ショートカットを使うと便利です。

.. code-block:: php

    $crawler->selectLink('Click here');

このコードにより、指定されたテキストを含むリンク、または、クリッカブルな画像のうちで、\ ``alt``\ 属性に指定されたテキストが含まれるものが選択されます。

Clientの\ ``click()``\  メソッドは、\ ``link()``\ メソッドから返された\ ``Link``\ インスタンスを引数にとります。

.. code-block:: php

    $link = $crawler->link();

    $client->click($link);

.. tip::

    ``links()``\ メソッドは、すべてのノードの\ ``Link``\  オブジェクトの配列を返します。

フォーム
~~~~~~~~

リンクと同じように、\ ``selectButton()``\ メソッドを使ってフォームを選択できます。

.. code-block:: php

    $crawler->selectButton('submit');

この処理では、フォーム自体ではなく、フォームのボタンを選択していることに注意してください。フォームには複数のボタンが存在する可能性があります。走査APIを使う際に、単一のボタンを特定する必要があることを覚えておいてください。

``selectButton()``\ メソッドで\ ``button``\  タグを選択し、\ ``input``\
タグの内容を送信します。これらを見つけるには、いくつかの方法があります。

* ``value``\ 属性の値

* 画像の\ ``id``\ または\ ``alt``\ 属性の値

* ``button``\ タグの\ ``id``\ または\ ``name``\ 属性の値

ボタンに対応するノードが見つかった場合、\ ``form()``\ メソッドを呼び出すと、ボタンノードを囲んでいる\ ``Form``\ インスタンスを取得できます。

.. code-block:: php

    $form = $crawler->form();

``form()``\ メソッドを呼び出す際に、フィールドの値を配列として渡すことで、フォームのデフォルト値を上書きできます。

.. code-block:: php

    $form = $crawler->form(array(
        'name'         => 'Fabien',
        'like_symfony' => true,
    ));

また、フォームで特定のHTTPメソッドをシミュレートしたい場合は、2つ目の引数に指定します。

.. code-block:: php

    $form = $crawler->form(array(), 'DELETE');

Clientから\ ``Form``\ インスタンスを送信します。

.. code-block:: php

    $client->submit($form);

フィールドの値は、\ ``submit()``\ メソッドの2つ目の引数で渡すこともできます。

.. code-block:: php

    $client->submit($form, array(
        'name'         => 'Fabien',
        'like_symfony' => true,
    ));

さらに複雑な状況の場合は、\ ``Form``\ インスタンスを配列のようにアクセスして、各フィールドの値を個別に設定できます。

::

    // フィールドの値を変更
    $form['name'] = 'Fabien';

フィールドのタイプごとに、値を操作する便利なAPIが用意されています。

.. code-block:: php

    // radioのオプションを選択
    $form['country']->select('France');

    // checkboxをチェック
    $form['like_symfony']->tick();

    // ファイルをアプロード
    $form['photo']->upload('/path/to/lucas.jpg');

.. tip::

    フォームに送信される値は\ ``getValues()``\ メソッドで取得できます。
    アップロードされたファイルにアクセスするには、\ ``getFiles()``\ メソッドの戻り値の配列を使います。\ ``getPhpValues()``\ と\ ``getPhpFiles()``\ は、送信された値をPHPフォーマットで返します（各括弧記法のキーをPHPの配列へ変換します）。

.. index::
   pair: Tests; Configuration

テストの設定
------------

.. index::
   pair: PHPUnit; Configuration

PHPUnitの設定
~~~~~~~~~~~~~

各アプリケーションごとにPHPUnitの設定があり、\ ``phpunit.xml.dist``\ ファイルに記述されています。このファイルを編集してデフォルト値を変更したり、\ ``phpunit.xml``\ ファイルを作成してローカルマシン用に設定をカスタマイズできます。

.. tip::

    コードリポジトリには\ ``phpunit.xml.dist``\ ファイルを保存し、\ ``phpunit.xml``\ ファイルは無視するよう設定してください。

デフォルトでは、\ ``phpunit``\ コマンドを実行した時に、"標準" バンドル内のテストだけが実行されます（標準とは、Vendor\\*Bundle\\Tests 名前空間を指します）。
対象の名前空間は簡単に追加できます。たとえば、次のように設定すると、インストールされたサードパーティのバンドルにあるテストが追加されます。

.. code-block:: xml

    <!-- hello/phpunit.xml.dist -->
    <testsuites>
        <testsuite name="Project Test Suite">
            <directory>../src/*/*Bundle/Tests</directory>
            <directory>../src/Acme/Bundle/*Bundle/Tests</directory>
        </testsuite>
    </testsuites>

コードカバレッジに別の名前空間を追加するには、\ ``<filter>``\ セクションを次のように編集してください。

.. code-block:: xml

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

Clientの設定
~~~~~~~~~~~~

ファンクショナルテストで使うClientから、Kernelが作られます。このKernelは、特殊な\ ``test``\ 環境で実行されるので、次のようにカスタマイズできます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        imports:
            - { resource: config_dev.yml }

        framework:
            error_handler: false
            test: ~

        web_profiler:
            toolbar: false
            intercept_redirects: false

        monolog:
            handlers:
                main:
                    type:  stream
                    path:  %kernel.logs_dir%/%kernel.environment%.log
                    level: debug

    .. code-block:: xml

        <!-- app/config/config_test.xml -->
        <container>
            <imports>
                <import resource="config_dev.xml" />
            </imports>

            <webprofiler:config
                toolbar="false"
                intercept-redirects="false"
            />

            <framework:config error_handler="false">
                <framework:test />
            </framework:config>

            <monolog:config>
                <monolog:main
                    type="stream"
                    path="%kernel.logs_dir%/%kernel.environment%.log"
                    level="debug"
                 />               
            </monolog:config>
        </container>

    .. code-block:: php

        // app/config/config_test.php
        $loader->import('config_dev.php');

        $container->loadFromExtension('framework', array(
            'error_handler' => false,
            'test'          => true,
        ));

        $container->loadFromExtension('web_profiler', array(
            'toolbar' => false,
            'intercept-redirects' => false,
        ));

        $container->loadFromExtension('monolog', array(
            'handlers' => array(
                'main' => array('type' => 'stream',
                                'path' => '%kernel.logs_dir%/%kernel.environment%.log'
                                'level' => 'debug')
           
        )));

``createClient()`` メソッドにオプションを渡すことで、デフォルトの環境 (``test``) やデバッグモードの値 (``true``) を変更できます。

.. code-block:: php

    $client = static::createClient(array(
        'environment' => 'my_test_env',
        'debug'       => false,
    ));

アプリケーションの動作がHTTPヘッダーに依存している場合、\ ``createClient()``\ メソッドの2つ目の引数で渡します。

.. code-block:: php

    $client = static::createClient(array(), array(
        'HTTP_HOST'       => 'en.example.com',
        'HTTP_USER_AGENT' => 'MySuperBrowser/1.0',
    ));

リクエストごとにHTTPヘッダーの値を変更することもできます。

.. code-block:: php

    $client->request('GET', '/', array(), array(
        'HTTP_HOST'       => 'en.example.com',
        'HTTP_USER_AGENT' => 'MySuperBrowser/1.0',
    ));

.. tip::

    独自のClientオブジェクトを使うには、\ ``test.client.class``\ パラメータで変更するか、\ ``test.client``\ サービスを定義します。

Cookbookの参考記事
------------------

* :doc:`/cookbook/testing/http_authentication`
* :doc:`/cookbook/testing/insulating_clients`
* :doc:`/cookbook/testing/profiling`
