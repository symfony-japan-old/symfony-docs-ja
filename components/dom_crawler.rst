.. index::
   single: DomCrawler
   single: Components; DomCrawler

.. note::

    * 対象バージョン：2.3 (2.0以降)
    * 翻訳更新日：2013/11/30

DomCrawlerコンポーネント
========================

    DomCrawlerコンポーネントはHTML文書やXML文書中のDOM要素の調査を手助けします。

.. note::

    DomCrawlerは、DOM構造の編集や HTML/XML の再ダンプといった複雑な処理を行うようには設計されていません。

インストール
------------

2通りの方法でインストール出来ます。

* :doc:`composerでインストール </components/using_components>` (``symfony/dom-crawler`` on `Packagist`_);
* 公式Gitリポジトリからインストール (https://github.com/symfony/DomCrawler).

使い方
------

:class:`Symfony\\Component\\DomCrawler\\Crawler` クラスはHTML/XML文書のフィルタリングやDOM操作をするメソッドを提供します。

Crawlerインスタンスは、DOMの要素を表す :phpclass:`DOMElement` クラスのインスタンスの集合です。

.. code-block:: php

    use Symfony\Component\DomCrawler\Crawler;

    $html = <<<'HTML'
    <!DOCTYPE html>
    <html>
        <body>
            <p class="message">Hello World!</p>
            <p>Hello Crawler!</p>
        </body>
    </html>
    HTML;

    $crawler = new Crawler($html);

    foreach ($crawler as $domElement) {
        print $domElement->nodeName;
    }

:class:`Symfony\\Component\\DomCrawler\\Link` クラス、 :class:`Symfony\\Component\\DomCrawler\\Form` クラスは、それぞれHTMLのリンク、フォームを表す特別なクラスです。

.. note::

    DomCrawlerはHTML構造中の間違いを自動的に補正して、仕様にマッチさせようとします。
    例えば ``<p>`` タグの中に ``<p>`` タグをネストすると、DomCrawlerはそれらを共通の親を持つ兄弟要素として解釈し直します。
    これはHTML5の仕様に従った正しい動作ですが、予期せぬ問題を引き起こす可能性もあります。
    DomCrawler本来の用途ではありませんが、DomCrawlerが解釈した\ :ref:`DOM構造をHTMLとしてダンプ <component-dom-crawler-dumping>`\ して、問題の原因を調べることが出来ます。

ノードのフィルタリング
~~~~~~~~~~~~~~~~~~~~~~~

XPathによるフィルタリングは非常に簡単です。

.. code-block:: php

    $crawler = $crawler->filterXPath('descendant-or-self::body/p');

.. tip::

    内部的には ``DOMXPath::query`` メソッドが実際にXPathクエリを実行しています。

CssSelectorコンポーネントがインストールされていれば、フィルタリングはもっと簡単です。
jQuery風のセレクタでDOMを走査することが出来ます。

.. code-block:: php

    $crawler = $crawler->filter('body > p');

無名関数を使ったより複雑なフィルタリングも可能です。

.. code-block:: php

    use Symfony\Component\DomCrawler\Crawler;
    // ...

    $crawler = $crawler->filter('body > p')->reduce(function (Crawler $node, $i) {
        // 偶数番目のノードをフィルタ
        return ($i % 2) == 0;
    });

無名関数がfalseを返すことで、ノードをフィルタすることが出来ます。

.. note::

    どのフィルタリングメソッドも、ノードをフィルタした新しい :class:`Symfony\\Component\\DomCrawler\\Crawler` インスタンスを生成して返します。

ノードの走査
~~~~~~~~~~~~

選択済みのノード中の位置を指定してノードを選択

.. code-block:: php

    $crawler->filter('body > p')->eq(0);

選択済みのノードのうち最初と最後のノードを選択

.. code-block:: php

    $crawler->filter('body > p')->first();
    $crawler->filter('body > p')->last();

選択済みのノードと同レベルのノード（兄弟ノード）を選択

.. code-block:: php

    $crawler->filter('body > p')->siblings();

選択済みのノードより前または後にある、同レベルのノードを選択

.. code-block:: php

    $crawler->filter('body > p')->nextAll();
    $crawler->filter('body > p')->previousAll();

全ての子ノードまたは全ての親ノードを選択

.. code-block:: php

    $crawler->filter('body')->children();
    $crawler->filter('body > p')->parents();

.. note::

    どの走査メソッドも、新しい :class:`Symfony\\Component\\DomCrawler\\Crawler` インスタンスを生成して返します。

ノードの値の取得
~~~~~~~~~~~~~~~~

選択済みのノードのうち最初のノードの内包するテキストを取得

.. code-block:: php

    $message = $crawler->filterXPath('//body/p')->text();

選択済みのノードのうち最初のノードの属性値を取得

.. code-block:: php

    $class = $crawler->filterXPath('//body/p')->attr('class');

選択済みのノードのテキストと属性値を配列として取得

.. code-block:: php

    $attributes = $crawler
        ->filterXpath('//body/p')
        ->extract(array('_text', 'class'))
    ;

.. note::

    特殊属性 ``_text`` はノードのテキストを表します。

ノードリスト中の各要素に対して順番に無名関数を実行する

.. code-block:: php

    use Symfony\Component\DomCrawler\Crawler;
    // ...

    $nodeValues = $crawler->filter('p')->each(function (Crawler $node, $i) {
        return $node->text();
    });

.. versionadded:: 2.3
    Symfony 2.3 では、 ``each``, ``reduce`` メソッドに渡す無名関数は第1引数として ``Crawler``
    オブジェクトを受け取ります。以前のバージョンでは第1引数は :phpclass:`DOMNode` でした。

無名関数は引数として ``Crawler`` オブジェクトと、ノードリスト中の位置を受け取ります。
無名関数が返す値の配列が ``each`` メソッドの返り値になります。

コンテンツの追加
~~~~~~~~~~~~~~~~

Crawlerはノードにコンテンツを追加する様々な方法をサポートします。

.. code-block:: php

    $crawler = new Crawler('<html><body /></html>');

    $crawler->addHtmlContent('<html><body /></html>');
    $crawler->addXmlContent('<root><node /></root>');

    $crawler->addContent('<html><body /></html>');
    $crawler->addContent('<root><node /></root>', 'text/xml');

    $crawler->add('<html><body /></html>');
    $crawler->add('<root><node /></root>');

.. note::

    ISO-8859-1 以外の文字コードを扱う場合は、
    HTMLコンテンツを追加するには
    :method:`Symfony\\Component\\DomCrawler\\Crawler::addHTMLContent`
    メソッドの第2引数に文字コードを指定して使います。

CrawlerはDOM拡張として実装されているため、
:phpclass:`DOMDocument`, :phpclass:`DOMNodeList`, :phpclass:`DOMNode` の各インスタンスを利用することも出来ます。

.. code-block:: php

    $document = new \DOMDocument();
    $document->loadXml('<root><node /><node /></root>');
    $nodeList = $document->getElementsByTagName('node');
    $node = $document->getElementsByTagName('node')->item(0);

    $crawler->addDocument($document);
    $crawler->addNodeList($nodeList);
    $crawler->addNodes(array($node));
    $crawler->addNode($node);
    $crawler->add($document);

.. _component-dom-crawler-dumping:

.. sidebar:: Manipulating and Dumping a ``Crawler``

    これら ``Crawler`` のメソッドは複雑なDOM操作を目的として作られたわけではありません。
    しかし ``Crawler`` は :phpclass:`DOMElement` クラスのインスタンスの集合であるため、
    :phpclass:`DOMElement`, :phpclass:`DOMNode`, :phpclass:`DOMDocument` などのメソッドやプロパティを利用出来ます。
    例えばこのようにして、 ``Crawler`` にHTMLをダンプさせることも可能です。

    .. code-block:: php

        $html = '';

        foreach ($crawler as $domElement) {
            $html .= $domElement->ownerDocument->saveHTML($domElement);
        }

    もしくは :method:`Symfony\\Component\\DomCrawler\\Crawler::html` メソッドを使ってHTMLを取得出来ます。

    .. code-block:: php

        $html = $crawler->html();

    ``html`` メソッドは Symfony 2.3 で導入されました。

リンク
~~~~~~

``Crawler`` の ``selectLink`` メソッドを使って、テキストリンクの文字列を指定して（もしくは画像リンクのalt属性の文字列を指定して）リンクを取得することが出来ます。
このメソッドはリンクのノードを含む ``Crawler`` オブジェクトを返します。
この ``Crawler`` オブジェクトの ``link()`` メソッドを呼ぶことで、 :class:`Symfony\\Component\\DomCrawler\\Link` のオブジェクトを取得出来ます。

.. code-block:: php

    $linksCrawler = $crawler->selectLink('Go elsewhere...');
    $link = $linksCrawler->link();

    // 上記を1行で
    $link = $crawler->selectLink('Go elsewhere...')->link();

:class:`Symfony\\Component\\DomCrawler\\Link` オブジェクトのメソッドを使って、リンクの情報を取得出来ます。

.. code-block:: php

    // リンクのURIを返す
    $uri = $link->getUri();

.. note::

    ``getUri()`` は、リンクの ``href`` 属性の値を完全なURIに変換して返してくれます。
    例えば ``href="#foo"`` のようなリンクであれば、現在のURIの末尾に ``#foo`` を付加した完全なURIを返します。

フォーム
~~~~~~~~

フォーム要素を便利に扱うための特別なメソッドが用意されています。
``Crawler`` の ``selectButton()`` メソッドで、ボタンのテキストを指定してボタン要素 (``input[type=submit]``, ``input[type=image]``, ``button``) の ``Crawler`` オブジェクトを取得出来ます。
この ``Crawler`` オブジェクトから、フォーム要素を表す :class:`Symfony\\Component\\DomCrawler\\Form` オブジェクトが取得出来ます。

.. code-block:: php

    $form = $crawler->selectButton('validate')->form();

    // フォームのフィールドにデータを入力する
    $form = $crawler->selectButton('validate')->form(array(
        'name' => 'Ryan',
    ));

:class:`Symfony\\Component\\DomCrawler\\Form` オブジェクトには多くの便利なメソッドが用意されています。

.. code-block:: php

    $uri = $form->getUri();

    $method = $form->getMethod();

:method:`Symfony\\Component\\DomCrawler\\Form::getUri` メソッドは単に ``action`` 属性の値を返すだけではありません。
GETメソッドの場合は、フォームの入力値に基づいたクエリ文字列を付加したURIを返してくれます。

フォームに仮想的に値を入力したり取得したり出来ます。

.. code-block:: php

    // 値を入力
    $form->setValues(array(
        'registration[username]' => 'symfonyfan',
        'registration[terms]'    => 1,
    ));

    // 値の配列を、上の例のようなフラットな配列で取得
    $values = $form->getValues();

    // 値の配列を、$_GET や $_POST で取得できるような形で取得
    // "registration" は配列になります
    $values = $form->getPhpValues();

以下の様な、フィールドが多次元配列のフォームを扱う場合は、

.. code-block:: html

    <form>
        <input name="multi[]" />
        <input name="multi[]" />
        <input name="multi[dimensional]" />
    </form>

値を多次元配列で渡します。

.. code-block:: php

    // 単一のフィールドに入力
    $form->setValues(array('multi' => array('value')));

    // 複数のフィールドに一度に入力
    $form->setValues(array('multi' => array(
        1             => 'value',
        'dimensional' => 'an other value'
    )));

``Form`` オブジェクトには、ラジオボタンやチェックボックスの選択、ファイルのアップロードなど、ブラウザのようにフォームを操作する方法が用意されており、より簡単にデータを入力することが出来ます。

.. code-block:: php

    $form['registration[username]']->setValue('symfonyfan');

    // チェックボックスを選択、非選択
    $form['registration[terms]']->tick();
    $form['registration[terms]']->untick();

    // option を選択
    $form['registration[birthday][year]']->select(1984);

    // "multiple" select の option を複数選択
    $form['registration[interests]']->select(array('symfony', 'cookies'));

    // ファイルアップロード
    $form['registration[photo]']->upload('/path/to/lucas.jpg');

フォームのデータを利用する
..........................

こうしてデータを入力した ``Form`` オブジェクトから、フォームを実際に送信した際にアプリケーションが受け取るデータを取得することが出来ます。

.. code-block:: php

    $values = $form->getPhpValues();
    $files = $form->getPhpFiles();

外部のHTTPクライアントを利用してテストする場合でも、POSTリクエストを生成するための全ての情報を ``Form`` オブジェクトから取得出来ます。

.. code-block:: php

    $uri = $form->getUri();
    $method = $form->getMethod();
    $values = $form->getValues();
    $files = $form->getFiles();

    // こうして取得したデータを外部のHTTPクライアントに渡します

これらを利用出来る、素晴らしいツールの1つに `Goutte`_ があります。
Goutte は Symfony の Crawler を直接受け取り、フォームを送信することが出来ます。

.. code-block:: php

    use Goutte\Client;

    // 外部のウェブサイトにリクエストを送信
    $client = new Client();
    $crawler = $client->request('GET', 'https://github.com/login');

    // フォームを取得してデータを入力
    $form = $crawler->selectButton('Log in')->form();
    $form['login'] = 'symfonyfan';
    $form['password'] = 'anypass';

    // フォームを送信
    $crawler = $client->submit($form);

.. _components-dom-crawler-invalid:

選択フィールドに不正な値をセットする
........................

.. versionadded:: 2.4
    :method:`Symfony\\Component\\DomCrawler\\Form::disableValidation` メソッドは Symfony 2.4 で追加されました。

選択フィールド (select, radio) では、不正な値がセット出来ないようバリデーションが有効になっています。
フォーム全体か単一のフィールドに対して ``disableValidation()`` メソッドを呼ぶことで、このバリデーションを無効にして不正な値をセットすることが出来ます。

.. code-block:: php

    // 単一のフィールドのバリデーションを無効にする
    $form['country']->disableValidation()->select('Invalid value');

    // フォーム全体のバリデーションを無効にする
    $form->disableValidation();
    $form['country']->select('Invalid value');

.. _`Goutte`:  https://github.com/fabpot/goutte
.. _Packagist: https://packagist.org/packages/symfony/dom-crawler

.. 2013/11/30 monmonmon fb61db9db5e777b8108e5f7aa01aceac8938ffad
