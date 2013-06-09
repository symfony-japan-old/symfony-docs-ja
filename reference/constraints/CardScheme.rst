CardScheme
==========

.. note::

    * 対象バージョン：2.3（2.2以降）
    * 翻訳更新日：2013/6/7

.. versionadded:: 2.2
    CardScheme バリデーションは Symfony 2.2 以降で利用可能です。

この制約を適用すると、指定したクレジットカード会社のカード番号として有効であることを検証できます。ペイメントゲートウェイでの支払い処理を実施する前にカード番号が有効であることを検証する、といった使い方ができます。

+----------------+--------------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`              |
+----------------+--------------------------------------------------------------------------+
| オプション     | - `schemes`_                                                             |
|                | - `message`_                                                             |
+----------------+--------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\CardScheme`          |
+----------------+--------------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\CardSchemeValidator` |
+----------------+--------------------------------------------------------------------------+

基本的な使い方
--------------

``CardScheme`` バリデーターを使うには、単純にクレジットカード番号を持つプロパティかメソッドへ制約を指定します。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/SubscriptionBundle/Resources/config/validation.yml
        Acme\SubscriptionBundle\Entity\Transaction:
            properties:
                cardNumber:
                    - CardScheme:
                        schemes: [VISA]
                        message: Your credit card number is invalid.

    .. code-block:: xml

        <!-- src/Acme/SubscriptionBundle/Resources/config/validation.xml -->
        <class name="Acme\SubscriptionBundle\Entity\Transaction">
            <property name="cardNumber">
                <constraint name="CardScheme">
                    <option name="schemes">
                        <value>VISA</value>
                    </option>
                    <option name="message">Your credit card number is invalid.</option>
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
             * @Assert\CardScheme(schemes = {"VISA"}, message = "Your credit card number is invalid.")
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
                $metadata->addPropertyConstraint('cardNumber', new Assert\CardScheme(array(
                    'schemes' => array(
                        'VISA'
                    ),
                    'message' => 'Your credit card number is invalid.',
                )));
            }
        }

利用可能なオプション
--------------------

schemes
-------

**型**: ``mixed`` [:ref:`default option<validation-default-option>`]

このオプションは必須で、クレジットカード番号の検証に使う番号スキーム名を、文字列または配列で指定します。指定できる番号スキーム名は、次のとおりです。

* ``AMEX``
* ``CHINA_UNIONPAY``
* ``DINERS``
* ``DISCOVER``
* ``INSTAPAYMENT``
* ``JCB``
* ``LASER``
* ``MAESTRO``
* ``MASTERCARD``
* ``VISA``

番号スキームについての詳細は、\ `Wikipedia: Issuer identification number (IIN)`_ を参照してください。

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``Unsupported card type or invalid card number``

値が ``CardScheme`` チェックで無効な場合に、このメッセージが表示されます。

.. _`Wikipedia: Issuer identification number (IIN)`: http://en.wikipedia.org/wiki/Bank_card_number#Issuer_identification_number_.28IIN.29

.. 2013/06/07 hidenorigoto 50e14fb2da08064642d359a9b23f8340ae5b83e0
