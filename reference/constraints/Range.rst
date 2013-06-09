Range
=====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

検証対象の数値が、指定した最大値と最小値の間にあることを検証します。

+----------------+---------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`         |
+----------------+---------------------------------------------------------------------+
| オプション     | - `min`_                                                            |
|                | - `max`_                                                            |
|                | - `minMessage`_                                                     |
|                | - `maxMessage`_                                                     |
|                | - `invalidMessage`_                                                 |
+----------------+---------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Range`          |
+----------------+---------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\RangeValidator` |
+----------------+---------------------------------------------------------------------+

基本的な使い方
--------------

クラスのh ``height``フィールドの値が ``120`` と ``180`` の間であることを保証するには、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                height:
                    - Range:
                        min: 120
                        max: 180
                        minMessage: You must be at least 120cm tall to enter
                        maxMessage: You cannot be taller than 180cm to enter

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        namespace Acme\EventBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Range(
             *      min = 120,
             *      max = 180,
             *      minMessage = "You must be at least 120cm tall to enter",
             *      maxMessage = "You cannot be taller than 180cm to enter"
             * )
             */
             protected $height;
        }

    .. code-block:: xml

        <!-- src/Acme/EventBundle/Resources/config/validation.xml -->
        <class name="Acme\EventBundle\Entity\Participant">
            <property name="height">
                <constraint name="Range">
                    <option name="min">120</option>
                    <option name="max">180</option>
                    <option name="minMessage">You must be at least 120cm tall to enter</option>
                    <option name="maxMessage">You cannot be taller than 180cm to enter</option>
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
                $metadata->addPropertyConstraint('height', new Assert\Range(array(
                    'min'        => 120,
                    'max'        => 180,
                    'minMessage' => 'You must be at least 120cm tall to enter',
                    'maxMessage' => 'You cannot be taller than 180cm to enter',
                )));
            }
        }

オプション
----------

min
~~~

**型**: ``integer`` [:ref:`default option<validation-default-option>`]

このオプションは必須で、有効範囲の最小値を指定します。検証対象の値がこの値よりも\ **小さい**\ 場合、検証は失敗します。

max
~~~

**型**: ``integer`` [:ref:`default option<validation-default-option>`]

このオプションは必須で、有効範囲の最大値を指定します。検証対象の値がこの値より\ **大きい**\ 場合、検証は失敗します。

minMessage
~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``This value should be {{ limit }} or more.``

検証対象の値が `min`_ オプションで指定した値より小さい場合に、このメッセージが表示されます。

maxMessage
~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``This value should be {{ limit }} or less.``

検証対象の値が `max`_ オプションで指定した値より大きい場合に、このメッセージが表示されます。

invalidMessage
~~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``This value should be a valid number.``

検証対象の値が数値ではない場合に、このメッセージが表示されます。数値かどうかは  `is_numeric`_ PHP 関数でチェックされます。

.. _`is_numeric`: http://www.php.net/manual/en/function.is-numeric.php

.. 2013/06/09 hidenorigoto 80a02647a7f66927edc3bf3401ce657a742f9a1a
