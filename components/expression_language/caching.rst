.. index::
    single: Caching; ExpressionLanguage

.. note::

    * 対象バージョン：2.4以降
    * 翻訳更新日：2013/12/19

パーサーキャッシュによる式のキャッシュ
======================================

ExpressionLanguage コンポーネントでは、\ :method:`Symfony\\Component\\ExpressionLanguage\\ExpressionLanguage::compile`
メソッドを使ってプレーン PHP にコンパイルされた式をキャッシュできます。
しかし、コンポーネント内部でもパース済みの式のキャッシュが行われるので、同じ式の 2 度目以降のコンパイルや評価は高速になります。

ワークフロー
------------

式の ``evaluate`` および ``compile`` において、結果の値を得るには何らかの前処理が必要です。
この 2 つのうち、\ ``evaluate`` はより多くのコストがかかります。

どちらのメソッドでも、式に対する字句解析と構文解析が行われます。この処理は 
:method:`Symfony\\Component\\ExpressionLanguage\\ExpressionLanguage::parse`
メソッドで行われ、\ :class:`Symfony\\Component\\ExpressionLanguage\\ParsedExpression` のインスタンスが返されます。
``compile`` メソッドでは、このオブジェクトの文字列表現が単純に返されます。
``evaluate`` メソッドでは、"ノード" (``ParsedExpression`` インスタンスに保存された式の断片) をループしてその場で評価が行われます。

処理時間を短縮するために、\ ``ExpressionLanguage`` コンポーネントでは ``ParsedExpression`` インスタンスがキャッシュされます。
これにより、同一の式に対する 2 度目以降の字句解析と構文解析がスキップされます。
キャッシュには
:class:`Symfony\\Component\\ExpressionLanguage\\ParserCache\\ParserCacheInterface`
インターフェイスを実装するクラス、デフォルトでは
:class:`Symfony\\Component\\ExpressionLanguage\\ParserCache\\ArrayParserCache` が使われます。
キャッシュ戦略をカスタマイズするには、独自の ``ParserCache`` を定義して ``ExpressionLanguage`` オブジェクトへ注入します。

.. code-block:: php

    use Symfony\Component\ExpressionLanguage\ExpressionLanguage;
    use Acme\ExpressionLanguage\ParserCache\MyDatabaseParserCache;

    $cache = new MyDatabaseParserCache(...);
    $language = new ExpressionLanguage($cache);

.. note::

    `DoctrineBridge`_ には `doctrine キャッシュライブラリ`_ を使った ParserCache 実装があります。
    このライブラリでは、Apc やファイルシステムといった様々なキャッシュ戦略が提供されています。

パース済みのシリアライズされた式の使用
--------------------------------------

``evaluate`` および ``compile`` では、\ ``ParsedExpression`` と ``SerializedParsedExpression`` の両方を扱えます。

.. code-block:: php

    use Symfony\Component\ExpressionLanguage\ParsedExpression;
    // ...

    $expression = new ParsedExpression($language->parse('1 + 4'));

    echo $language->evaluate($expression); // 5 と表示

.. code-block:: php

    use Symfony\Component\ExpressionLanguage\SerializedParsedExpression;
    // ...

    $expression = new SerializedParsedExpression(serialize($language->parse('1 + 4')));

    echo $language->evaluate($expression); // 5 と表示

.. _DoctrineBridge: https://github.com/symfony/DoctrineBridge
.. _`doctrine キャッシュライブラリ`: http://docs.doctrine-project.org/projects/doctrine-common/en/latest/reference/caching.html

.. 2013/12/20 hidenorigoto be60602ac81e851e1e4b10b423c618a5a18aad2a
