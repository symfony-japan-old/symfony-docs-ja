Count
=====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/7

検証対象のコレクション（配列、または Countable を実装したオブジェクト）の要素数が指定された最小と最大の範囲内かどうかを検証します。

+----------------+---------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`         |
+----------------+---------------------------------------------------------------------+
| オプション     | - `min`_                                                            |
|                | - `max`_                                                            |
|                | - `minMessage`_                                                     |
|                | - `maxMessage`_                                                     |
|                | - `exactMessage`_                                                   |
+----------------+---------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Count`          |
+----------------+---------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\CountValidator` |
+----------------+---------------------------------------------------------------------+

基本的な使い方
--------------

``emails`` 配列フィールドに含まれる要素の数が 1 個から 5 個の間であることを検証するには、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                emails:
                    - Count:
                        min: 1
                        max: 5
                        minMessage: "You must specify at least one email"
                        maxMessage: "You cannot specify more than {{ limit }} emails"

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        namespace Acme\EventBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Count(
             *      min = "1",
             *      max = "5",
             *      minMessage = "You must specify at least one email",
             *      maxMessage = "You cannot specify more than {{ limit }} emails"
             * )
             */
             protected $emails = array();
        }

    .. code-block:: xml

        <!-- src/Acme/EventBundle/Resources/config/validation.xml -->
        <class name="Acme\EventBundle\Entity\Participant">
            <property name="emails">
                <constraint name="Count">       
                    <option name="min">1</option> 
                    <option name="max">5</option> 
                    <option name="minMessage">You must specify at least one email</option>
                    <option name="maxMessage">You cannot specify more than {{ limit }} emails</option>
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
            public static function loadValidatorMetadata(ClassMetadata $data)
            {
                $metadata->addPropertyConstraint('emails', new Assert\Count(array(
                    'min'        => 1,
                    'max'        => 5,
                    'minMessage' => 'You must specify at least one email',
                    'maxMessage' => 'You cannot specify more than {{ limit }} emails',
                )));
            }
        }

オプション
----------

min
~~~

**型**: ``integer`` [:ref:`default option<validation-default-option>`]

必須のオプションで、要素の最小個数を指定します。検証対象のコレクションの要素数が指定した個数\ **未満**\ である場合に、検証は失敗します。

max
~~~

**型**: ``integer`` [:ref:`default option<validation-default-option>`]

必須のオプションで、要素の最大個数を指定します。検証対象のコレクションの要素数が指定した個数\ **より多い**\ 場合に、検証は失敗します。

minMessage
~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``This collection should contain {{ limit }} elements or more.``.

検証対象のコレクションの要素数が `min`_ オプションで指定した値未満である場合に、このメッセージが表示されます。

maxMessage
~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``This collection should contain {{ limit }} elements or less.``.

検証対象のコレクションの要素数が `max`_ オプションで指定した値よい大きい場合に、このメッセージが表示されます。

exactMessage
~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``This collection should contain exactly {{ limit }} elements.``.

`max`_ オプションと `min`_ オプションの値が同じで、検証対象のコレクションの要素数がこれとは異なる場合に、このメッセージが表示されます。

.. 2013/06/07 hidenorigoto 4346f75f05a5ee010d0148ea251e99c7f6a02c38
