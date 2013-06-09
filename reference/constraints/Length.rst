Length
======
.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

検証対象の文字列の長さが、指定された最小と最大の\ *間*\ であることを検証します。

+----------------+----------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`          |
+----------------+----------------------------------------------------------------------+
| オプション     | - `min`_                                                             |
|                | - `max`_                                                             |
|                | - `charset`_                                                         |
|                | - `minMessage`_                                                      |
|                | - `maxMessage`_                                                      |
|                | - `exactMessage`_                                                    |
+----------------+----------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Length`          |
+----------------+----------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\LengthValidator` |
+----------------+----------------------------------------------------------------------+

基本的な使い方
--------------

クラスの ``firstName`` フィールドの文字列の長さが "2" と "50" の間であることを検証するには、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                firstName:
                    - Length:
                        min: 2
                        max: 50
                        minMessage: "Your first name must be at least {{ limit }} characters length"
                        maxMessage: "Your first name cannot be longer than {{ limit }} characters length"

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        namespace Acme\EventBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Length(
             *      min = "2",
             *      max = "50",
             *      minMessage = "Your first name must be at least {{ limit }} characters length",
             *      maxMessage = "Your first name cannot be longer than {{ limit }} characters length"
             * )
             */
             protected $firstName;
        }

    .. code-block:: xml

        <!-- src/Acme/EventBundle/Resources/config/validation.xml -->
        <class name="Acme\EventBundle\Entity\Participant">
            <property name="firstName">
                <constraint name="Length">
                    <option name="min">2</option>
                    <option name="max">50</option>
                    <option name="minMessage">Your first name must be at least {{ limit }} characters length</option>
                    <option name="maxMessage">Your first name cannot be longer than {{ limit }} characters length</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/EventBundle/Entity/Participant.php
        namespace Acme\EventBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', new Assert\Length(array(
                    'min'        => 2,
                    'max'        => 50,
                    'minMessage' => 'Your first name must be at least {{ limit }} characters length',
                    'maxMessage' => 'Your first name cannot be longer than {{ limit }} characters length',
                )));
            }
        }

オプション
----------

min
~~~

**型**: ``integer`` [:ref:`default option<validation-default-option>`]

このオプションは必須で、文字列の長さの最小値を指定します。検証対象の文字列の長さが指定した値\ **より短い**\ 場合、検証に失敗します。

max
~~~

**型**: ``integer`` [:ref:`default option<validation-default-option>`]

このオプションは必須で、文字列の長さの最大値を指定します。検証対象の文字列の長さが指定した値\ **より長い**\ 場合、検証に失敗します。

charset
~~~~~~~

**型**: ``string``  **デフォルト**: ``UTF-8``

検証対象の文字列の長さを計算する際に内部で使われる文字セットを指定します。長さの計算には、利用可能であれば :phpfunction:`grapheme_strlen` PHP 関数が使われます。そうでなければ :phpfunction:`mb_strlen` PHP 関数が使われます。どちらも利用できなければ :phpfunction:`strlen` PHP 関数が使われます。

minMessage
~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``This value is too short. It should have {{ limit }} characters or more.``.

検証対象の文字列の長さが `min`_ オプションで指定した長さより短い場合に、このメッセージが表示されます。

maxMessage
~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``This value is too long. It should have {{ limit }} characters or less.``.

検証対象の文字列の長さが `max`_ オプションで指定した長さより長い場合に、このメッセージが表示されます。

exactMessage
~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``This value should have exactly {{ limit }} characters.``.

`min`_ オプションと `max`_ オプションの値が同じで、検証対象の文字列の長さが異なる場合に、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto bf6751719dc84ba61ccf737a496c49f4e830d7cc
