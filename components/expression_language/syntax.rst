.. index::
    single: Syntax; ExpressionLanguage

.. note::

    * 対象バージョン：2.4以降
    * 翻訳更新日：2013/12/19

式の構文
========

ExpressionLanguage コンポーネントでは、Twig をもとにした式の構文を採用しています。
サポートしているすべての構文について解説します。

リテラル
--------

次のリテラルをサポートしています:

* **文字列** - シングルクォートまたはダブルクォートで囲みます (``'hello'``)
* **数値** - ``103``
* **配列** - JSON に似た表記 (``[1, 2]``)
* **連想配列** - JSON に似た表記 (``{ foo: 'bar' }``)
* **ブール値** - ``true`` または ``false``
* **null** - ``null``

.. _component-expression-objects:

オブジェクトの使用
------------------

式へオブジェクトを渡した場合、プロパティへのアクセス、メソッドの呼び出し用の構文を使います。

パブリックプロパティへのアクセス
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

オブジェクトのパブリックプロパティへアクセスするには、\ ``.`` を使います。

.. code-block:: php

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

このコードを実行すると ``Honeycrisp`` と表示されます。

メソッド呼び出し
~~~~~~~~~~~~~~~~

オブジェクトのメソッド呼び出しにも ``.`` を使います。

.. code-block:: php

    class Robot
    {
        public function sayHi($times)
        {
            $greetings = array();
            for ($i = 0; $i < $times; $i++) {
                $greetings[] = 'Hi';
            }

            return implode(' ', $greetings).'!';
        }
    }

    $robot = new Robot();

    echo $language->evaluate(
        'robot.sayHi(3)',
        array(
            'robot' => $robot,
        )
    );

このコードを実行すると、\ ``Hi Hi Hi!`` と表示されます。

.. _component-expression-functions:

関数の使用
----------

式の中で登録済みの関数を使うこともできます。関数を呼び出す構文は PHP や JavaScript と共通です。
ExpressionLanguage コンポーネントには、PHP の定数値を返す ``constant()`` という関数がデフォルトで組み込まれています。

.. code-block:: php

    define('DB_USER', 'root');

    echo $language->evaluate(
        'constant("DB_USER")'
    );

このコードを実行すると ``root`` と表示されます。

.. tip::

    式で利用するために独自の関数を登録する方法は、\ ":doc:`/components/expression_language/extending`" を参照してください。

.. _component-expression-arrays:

配列の使用
----------

式に配列（または連想配列）を渡した場合、\ ``[]`` で配列の添字または連想配列のキーを指定して要素にアクセスできます。

.. code-block:: php

    $data = array('life' => 10, 'universe' => 10, 'everything' => 22);

    echo $language->evaluate(
        'data["life"] + data["universe"] + data["everything"]',
        array(
            'data' => $data,
        )
    );

このコードを実行すると ``42`` と表示されます。

サポートされる演算子
--------------------

多くの演算子がサポートされています:

算術演算子
~~~~~~~~~~

* ``+`` (加算)
* ``-`` (減算)
* ``*`` (乗算)
* ``/`` (除算)
* ``%`` (剰余)
* ``**`` (べき乗)

例

.. code-block:: php

    echo $language->evaluate(
        'life + universe + everything',
        array(
            'life' => 10,
            'universe' => 10,
            'everything' => 22,
        )
    );

このコードを実行すると ``42`` と表示されます。

ビット演算子
~~~~~~~~~~~~

* ``&`` (and)
* ``|`` (or)
* ``^`` (xor)

比較演算子
~~~~~~~~~~

* ``==`` (等価)
* ``===`` (同一)
* ``!=`` (非等価)
* ``!==`` (非同一)
* ``<`` (未満)
* ``>`` (より大きい)
* ``<=`` (以下)
* ``>=`` (以上)
* ``matches`` (正規表現にマッチ)

.. tip::

    文字列が正規表現に *マッチしない* ことをテストするには、\ ``match`` 演算子と論理 ``not`` 演算子を組み合わせて次のようにします。
    
    ..code_block:: php

        $language->evaluate('not "foo" matches "/bar/"'); // true が返る

例

.. code-block:: php

    $ret1 = $language->evaluate(
        'life == everything',
        array(
            'life' => 10,
            'universe' => 10,
            'everything' => 22,
        )
    );

    $ret2 = $language->evaluate(
        'life > everything',
        array(
            'life' => 10,
            'universe' => 10,
            'everything' => 22,
        )
    );

``$ret1`` と ``$ret2`` のいずれも ``false`` になります。

論理演算子
~~~~~~~~~~

* ``not`` または ``!``
* ``and`` または ``&&``
* ``or`` または ``||``

例

.. code-block:: php

    $ret = $language->evaluate(
        'life < universe or life < everything',
        array(
            'life' => 10,
            'universe' => 10,
            'everything' => 22,
        )
    );

``$ret`` は ``true`` になります。

文字列演算子
~~~~~~~~~~~~

* ``~`` (連結)

例

.. code-block:: php

    echo $language->evaluate(
        'firstName~" "~lastName',
        array(
            'firstName' => 'Arthur',
            'lastName' => 'Dent',
        )
    );

このコードを実行すると ``Arthur Dent`` と表示されます。

配列演算子
~~~~~~~~~~

* ``in`` (要素を含む)
* ``not in`` (要素を含まない)

例

.. code-block:: php

    class User
    {
        public $group;
    }

    $user = new User();
    $user->group = 'human_resources';

    $inGroup = $language->evaluate(
        'user.group in ["human_resources", "marketing"]',
        array(
            'user' => $user
        )
    );

``$inGroup`` の値は ``true`` になります。

数値演算子
~~~~~~~~~~

* ``..`` (範囲)

例

.. code-block:: php

    class User
    {
        public $age;
    }

    $user = new User();
    $user->age = 34;

    $language->evaluate(
        'user.age in 18..45',
        array(
            'user' => $user,
        )
    );

``user.age`` は ``18`` から ``45`` の間にあるので、評価結果は ``true`` になります。

三項演算子
~~~~~~~~~~

* ``foo ? 'yes' : 'no'``
* ``foo ?: 'no'`` (``foo ? foo : 'no'`` と同じ)
* ``foo ? 'yes'`` (``foo ? 'yes' : ''`` と同じ)

.. 2013/12/20 hidenorigoto 667d590d1492ccfad9ba3b2117d77bfcce4720b6
