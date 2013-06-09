Choice
======

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/7

この制約を使うと、入力値が\ *有効*\ な選択肢のどれかと一致することを検証できます。同様に、配列のすべての要素に対して有効な選択肢と一致することを検証できます。

+----------------+-----------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`           |
+----------------+-----------------------------------------------------------------------+
| オプション     | - `choices`_                                                          |
|                | - `callback`_                                                         |
|                | - `multiple`_                                                         |
|                | - `min`_                                                              |
|                | - `max`_                                                              |
|                | - `message`_                                                          |
|                | - `multipleMessage`_                                                  |
|                | - `minMessage`_                                                       |
|                | - `maxMessage`_                                                       |
|                | - `strict`_                                                           |
+----------------+-----------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Choice`           |
+----------------+-----------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\ChoiceValidator`  |
+----------------+-----------------------------------------------------------------------+

基本的な使い方
--------------

この制約では、有効な値の配列を指定すると、プロパティの値が有効な値の配列にあるかどうか検証されます。有効な値の配列を指定する方法は、いくつかあります。

有効な選択肢リストが単純な場合は、次のように `choices`_ オプションを使って直接指定します。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice:
                        choices:  [male, female]
                        message:  Choose a valid gender.

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice(choices = {"male", "female"}, message = "Choose a valid gender.")
             */
            protected $gender;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="gender">
                <constraint name="Choice">
                    <option name="choices">
                        <value>male</value>
                        <value>female</value>
                    </option>
                    <option name="message">Choose a valid gender.</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/EntityAuthor.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;
        
        class Author
        {
            protected $gender;
            
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Assert\Choice(array(
                    'choices' => array('male', 'female'),
                    'message' => 'Choose a valid gender.',
                )));
            }
        }

コールバックを使って選択肢を指定する
------------------------------------

有効な選択肢リストを返すようなコールバック関数を使うこともできます。たとえば、選択肢リストをバリデーションとフォームの SELECT 要素の出力の両方に利用したい場合、コールバック関数を使えば選択肢リストを 1 箇所で管理できます。次のようにリストを返すメソッドをエンティティクラスに用意します。

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    class Author
    {
        public static function getGenders()
        {
            return array('male', 'female');
        }
    }

このメソッドの名前を ``Choice`` 制約の `callback`_ オプションに指定します。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice: { callback: getGenders }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice(callback = "getGenders")
             */
            protected $gender;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="gender">
                <constraint name="Choice">
                    <option name="callback">getGenders</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/EntityAuthor.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;
        
        class Author
        {
            protected $gender;
            
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Assert\Choice(array(
                    'callback' => 'getGenders',
                )));
            }
        }

たとえば ``Util`` クラスのように別のクラスに定義したスタティックメソッドをコールバックとして使いたい場合は、クラス名とメソッド名の配列として指定します。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice: { callback: [Util, getGenders] }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice(callback = {"Util", "getGenders"})
             */
            protected $gender;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="gender">
                <constraint name="Choice">
                    <option name="callback">
                        <value>Util</value>
                        <value>getGenders</value>
                    </option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/EntityAuthor.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;
        
        class Author
        {
            protected $gender;
            
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Assert\Choice(array(
                    'callback' => array('Util', 'getGenders'),
                )));
            }
        }

利用可能なオプション
--------------------

choices
~~~~~~~

**型**: ``array`` [:ref:`default option<validation-default-option>`]

必須（\ `callback`_ を指定した場合を除く） - 有効な選択肢として扱われる値の配列。この配列に対して入力値のマッチングが行われます。

callback
~~~~~~~~

**型**: ``string|array|Closure``

有効な選択肢を返すコールバックメソッドで、\ `choices`_ オプションの代わりに使われます。使い方は\ `コールバックを使って選択肢を指定する`_\ を参照してください。

multiple
~~~~~~~~

**型**: ``Boolean`` **デフォルト**: ``false``

このオプションに ``true`` を指定すると、入力値が単一のスカラ値ではなく配列であるとみなされます。入力値である配列の各要素の値が、有効な選択肢の中にあるかどうかチェックされます。有効な選択肢にない要素が 1 つでも見つかった場合、バリデーションは失敗します。

min
~~~

**型**: ``integer``

``multiple`` オプションが ``true`` の場合、選択される値の個数の最小値を ``min`` オプションで指定できます。たとえば ``min`` に 3 を指定し、入力値に有効な値が 2 つしかない場合、バリデーションは失敗します。

max
~~~

**型**: ``integer``

``multiple`` オプションが ``true`` の場合、選択される値の個数の最大値を ``max`` オプションで指定できます。たとえば ``max`` に 3 を指定し、入力値に有効な値が 4 つある場合、バリデーションは失敗します。

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``The value you selected is not a valid choice``

``multiple`` オプションが ``false`` で、入力値が有効な選択肢にない場合に、このメッセージが表示されます。

multipleMessage
~~~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``One or more of the given values is invalid``

``multiple`` オプションが ``true`` で、入力値の配列の要素の 1 つでも選択肢になかった場合に、このメッセージが表示されます。

minMessage
~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``You must select at least {{ limit }} choices``

ユーザーが選択した数が `min`_ オプションよりも少なかった場合に、このメッセージが表示されます。

maxMessage
~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``You must select at most {{ limit }} choices``

ユーザーが選択した数が `max`_ オプションよりも多かった場合に、このメッセージが表示されます。

strict
~~~~~~

**型**: ``Boolean`` **デフォルト**: ``false``

このオプションを `true` に設定すると、バリデーターにより入力値の型も検証されます。内部的には、値が有効な選択肢にあるかどうかを調べる時に PHP の :phpfunction:`in_array` 関数の第 3 引数として渡されます。

.. 2013/06/07 hidenoritoto eac3adc9e23141a00619b1812cbc6dda5025d1af
