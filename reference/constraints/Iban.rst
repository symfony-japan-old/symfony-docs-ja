Iban
====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

.. versionadded:: 2.3
    この制約はバージョン 2.3 以降で利用可能です。

この制約は、銀行口座番号の値が `International Bank Account Number (IBAN)`_ 形式として適切であることを保証します。IBAN は国際規格なので、国を超えて銀行口座を指定でき、口座番号のエラーが伝播していくリスクを低減させられます。

+----------------+-----------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`           |
+----------------+-----------------------------------------------------------------------+
| オプション     | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Iban`             |
+----------------+-----------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\IbanValidator`    |
+----------------+-----------------------------------------------------------------------+

基本的な使い方
--------------

IBAN バリデーターを使うには、オブジェクトの IBAN を含むプロパティに単純に制約を適用します。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/SubscriptionBundle/Resources/config/validation.yml
        Acme\SubscriptionBundle\Entity\Transaction:
            properties:
                bankAccountNumber:
                    - Iban:
                        message: This is not a valid International Bank Account Number (IBAN).

    .. code-block:: xml

        <!-- src/Acme/SubscriptionBundle/Resources/config/validation.xml -->
        <class name="Acme\SubscriptionBundle\Entity\Transaction">
            <property name="bankAccountNumber">
                <constraint name="Iban">
                    <option name="message">This is not a valid International Bank Account Number (IBAN).</option>
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
             * @Assert\Iban(message = "This is not a valid International Bank Account Number (IBAN).")
             */
            protected $bankAccountNumber;
        }

    .. code-block:: php

        // src/Acme/SubscriptionBundle/Entity/Transaction.php
        namespace Acme\SubscriptionBundle\Entity\Transaction;
        
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Transaction
        {
            protected $bankAccountNumber;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('bankAccountNumber', new Assert\Iban(array(
                    'message' => 'This is not a valid International Bank Account Number (IBAN).',
                )));
            }
        }

利用可能なオプション
--------------------

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This is not a valid International Bank Account Number (IBAN).``

値が IBAN チェックをパスしなかった場合に、このメッセージが表示されます。

.. _`International Bank Account Number (IBAN)`: http://en.wikipedia.org/wiki/International_Bank_Account_Number

.. 2013/06/09 hidenorigoto 57b4b002e537c89667952ea912223b3032856e43
