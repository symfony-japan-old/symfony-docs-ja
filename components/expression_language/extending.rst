.. index::
    single: Extending; ExpressionLanguage

.. note::

    * 対象バージョン：2.4以降
    * 翻訳更新日：2013/12/19

ExpressionLanguage の拡張
=========================

ExpressionLanguage にカスタム関数を追加して拡張できます。
たとえば Symfony フレームワークでは、ログインユーザーのロールをチェックするカスタム関数が追加されています。

.. note::

    式の中での関数の使い方については、
    ":ref:`component-expression-functions`" を参照してください。

関数の登録
----------

関数を使うには、\ ``ExpressionLanguage`` インスタンスごとに関数を登録する必要があります。
登録した関数は、\ ``ExpressionLanguage`` インスタンスで実行される任意の式で利用可能です。

関数を登録するには、
:method:`Symfony\\Component\\ExpressionLanguage\\ExpressionLanguage::register`` メソッドを使います。
このメソッドには次の 3 つの引数があります:

* **name** - 式の中で利用する関数の名前
* **compiler** - 関数を利用している式のコンパイルに利用する PHP 関数
* **evaluator** - 関数を利用している式の評価に利用する PHP 関数

.. code-block:: php

    use Symfony\Component\ExpressionLanguage\ExpressionLanguage;

    $language = new ExpressionLanguage();
    $language->register('lowercase', function ($str) {
        if (!is_string($str)) {
            return $str;
        }

        return sprintf('strtolower(%s)', $str);
    }, function ($str) {
        if (!is_string($str)) {
            return $str;
        }

        return strtolower($str);
    });

    echo $language->evaluate('lowercase("HELLO")');

このコードを実行すると ``hello`` と表示されます。

新しい ExpressionLanguage クラスの定義
--------------------------------------

自分のライブラリ内で ``ExpressionLanguage`` クラスを使う場合、専用の ``ExpressionLanguage`` クラスを定義し、そこに関数を登録することをおすすめします。
``registerFunctions`` をオーバーライドし、使う関数を追加します。

.. code-block:: php

    namespace Acme\AwesomeLib\ExpressionLanguage;

    use Symfony\Component\ExpressionLanguage\ExpressionLanguage as BaseExpressionLanguage;

    class ExpressionLanguage extends BaseExpressionLanguage
    {
        protected function registerFunctions()
        {
            parent::registerFunctions(); // コア関数の登録を行うために必要

            $this->register('lowercase', function ($str) {
                if (!is_string($str)) {
                    return $str;
                }

                return sprintf('strtolower(%s)', $str);
            }, function ($str) {
                if (!is_string($str)) {
                    return $str;
                }

                return strtolower($str);
            });
        }
    }

.. 20131220 hidenorigoto 667d590d1492ccfad9ba3b2117d77bfcce4720b6
