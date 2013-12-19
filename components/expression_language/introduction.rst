.. index::
    single: Expressions
    Single: Components; Expression Language

.. note::

    * 対象バージョン：2.4以降
    * 翻訳更新日：2013/12/19

ExpressionLanguage コンポーネント
=================================

    ExpressionLanguage コンポーネントを使うと、"式" をコンパイルおよび評価できます。
    "式" とは値を返すワンライナーです。多くの場合はブール型の値を返しますが、他の値を返すこともできます。

.. versionadded:: 2.4
    ExpressionLanguage コンポーネントは Symfony 2.4 以降で利用可能です。

以降では、Expression Language を式言語、Expression Engine を式言語エンジンと呼びます。

インストール
------------

次の 2 種類の方法でインストールできます。

* :doc:`Composer を使ってインストール </components/using_components>` (`Packagist`_ の ``symfony/expression-language``)
* オフィシャル Git リポジトリから (https://github.com/symfony/expression-language)

Expression Engine で何ができるのか？
------------------------------------

このコンポーネントにより、開発者はコンフィギュレーション中で式を使って複雑なロジックを記述できるようになります。たとえば、Symfony2 フレームワークではセキュリティやバリデーションのルール、ルートのマッチングに式を使えます。

ExpressionLanguage コンポーネントはフレームワーク内部で利用されるだけでなく、*ビジネスルールのエンジン* としても使えるでしょう。Web マスターが直接 PHP を書き換えなければならなようなセキュリティリスクをともなうことなく、ダイナミックに Web サイトをコンフィギュレーションする機能などに応用できます。

.. _component-expression-language-examples:

.. code-block:: text

    # 次の条件を満たすなら特別価格を取得する
    user.getGroup() in ['good_customers', 'collaborator']

    # 次の条件を満たすなら記事を宣伝する
    article.commentCount > 100 and article.category not in ["misc"]

    # 次の条件を満たすなら警告を送信する
    product.stock < 15

式は制限された PHP のサンドボックスとみなすことができます。式の中で使える変数は明示的に宣言されたものに限られるため、外部からの影響を受けません。

使い方
------

ExpressionLanguage コンポーネントで式をコンパイルし、評価できます。
式は通常ブール値を返すワンライナーで、\ ``if`` 文の条件として実行されるコードに使えます。
単純な式の例は ``1 + 2`` です。\ ``someArray[3].someMethod('bar')`` のようなより複雑な式も使えます。

式は、次の 2 とおりの方法で処理します。

* **評価**: PHP コードにコンパイルせず、直接式を評価します。
* **コンパイル**: 式が PHP コードにコンパイルされます。コンパイルされたコードをキャッシュしたり、評価したりできます。

コンポーネントの中心となるクラスは :class:`Symfony\\Component\\ExpressionLanguage\\ExpressionLanguage` です。

.. code-block:: php

    use Symfony\Component\ExpressionLanguage\ExpressionLanguage;

    $language = new ExpressionLanguage();

    echo $language->evaluate('1 + 2'); // 3 と表示される

    echo $language->compile('1 + 2'); // (1 + 2) と表示される

式言語の構文
------------

ExpressionLanguage コンポーネントの式言語の構文については :doc:`/components/expression_language/syntax` を参照してください。

変数を渡す
----------

式へ変数を渡すこともできます。オブジェクトを含む、PHP で利用可能な任意の型の変数を渡せます。

.. code-block:: php

    use Symfony\Component\ExpressionLanguage\ExpressionLanguage;

    $language = new ExpressionLanguage();

    class Apple
    {
        public $variety;
    }

    $apple = new Apple();
    $apple->variety = 'Honeycrisp';

    echo $language->evaluate(
        'fruit.variety',
        array(
            'fruit' => $apple,
        )
    );

このコードを実行すると "Honeycrisp" と表示されます。構文についての詳細は :doc:`/components/expression_language/syntax` および :ref:`component-expression-objects`\ 、\ :ref:`component-expression-arrays` を参照してください。

キャッシュ
----------

式のコンパイル結果のキャッシュ用に、複数の戦略が用意されています。キャッシュについては :doc:`/components/expression_language/caching` を参照してください。

.. _Packagist: https://packagist.org/packages/symfony/expression-language

.. 2013/12/19 hidenorigoto a9e0e66dd5967788a3b5862022e0080a9b2bb300
