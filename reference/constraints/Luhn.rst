Luhn
====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

.. versionadded:: 2.3
    この制約はバージョン 2.3 以降で利用可能です。

この制約は、クレジットカード番号が `Luhn アルゴリズム`_\ をパスすることを保証します。クレジットカード番号に対して、ペイメントゲートウェイに送る前の第 1 段階の検証に役立ちます。

+----------------+-----------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`           |
+----------------+-----------------------------------------------------------------------+
| オプション     | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Luhn`             |
+----------------+-----------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\LuhnValidator`    |
+----------------+-----------------------------------------------------------------------+

基本的な使い方
--------------

Luhn バリデーターを使うには、オブジェクトのクレジットカード番号を保持するプロパティに単純に適用します。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/SubscriptionBundle/Resources/config/validation.yml
        Acme\SubscriptionBundle\Entity\Transaction:
            properties:
                cardNumber:
                    - Luhn:
                        message: Please check your credit card number.

    .. code-block:: xml

        <!-- src/Acme/SubscriptionBundle/Resources/config/validation.xml -->
        <class name="Acme\SubscriptionBundle\Entity\Transaction">
            <property name="cardNumber">
                <constraint name="Luhn">
                    <option name="message">Please check your credit card number.</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/SubscriptionBundle/Entity/Transaction.php
        namespace Acme\SubscriptionBundle\Entity\Transaction;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Transaction
        {
            /**
             * @Assert\Luhn(message = "Please check your credit card number.")
             */
            protected $cardNumber;
        }

    .. code-block:: php

        // src/Acme/SubscriptionBundle/Entity/Transaction.php
        namespace Acme\SubscriptionBundle\Entity\Transaction;
        
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Transaction
        {
            protected $cardNumber;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('cardNumber', new Assert\Luhn(array(
                    'message' => 'Please check your credit card number',
                )));
            }
        }

利用可能なオプション
--------------------

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``Invalid card number``

検証対象の文字列が Luhn チェックをパスしなかった場合に、このメッセージが表示されます。

.. _`Luhn アルゴリズム`: http://en.wikipedia.org/wiki/Luhn_algorithm

.. 2013/06/09 hidenorigoto e6d6532d503ee356a06e72916a4177473bdaba68
