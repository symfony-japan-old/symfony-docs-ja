Issn
====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

.. versionadded:: 2.3
    この制約はバージョン 2.3 以降で利用可能です。

値が有効な `ISSN`_ かどうか検証します。

+----------------+-----------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`           |
+----------------+-----------------------------------------------------------------------+
| オプション     | - `message`_                                                          |
|                | - `caseSensitive`_                                                    |
|                | - `requireHyphen`_                                                    |
+----------------+-----------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Issn`             |
+----------------+-----------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\IssnValidator`    |
+----------------+-----------------------------------------------------------------------+

基本的な使い方
--------------

.. configuration-block::

    .. code-block:: yaml

        # src/JournalBundle/Resources/config/validation.yml
        Acme\JournalBundle\Entity\Journal:
            properties:
                issn:
                    - Issn: ~

    .. code-block:: php-annotations

        // src/Acme/JournalBundle/Entity/Journal.php
        namespace Acme\JournalBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Journal
        {
            /**
             * @Assert\Issn
             */
             protected $issn;
        }

    .. code-block:: xml

        <!-- src/Acme/JournalBundle/Resources/config/validation.xml -->
        <class name="Acme\JournalBundle\Entity\Journal">
            <property name="issn">
                <constraint name="Issn" />
            </property>
        </class>

    .. code-block:: php

        // src/Acme/JournalBundle/Entity/Journal.php
        namespace Acme\JournalBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Journal
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('issn', new Assert\Issn());
            }
        }

オプション
----------

message
~~~~~~~

**型**: ``String`` **デフォルト**: ``This value is not a valid ISSN.``

検証対象の値が有効な ISSN ではない場合に、このメッセージが表示されます。

caseSensitive
~~~~~~~~~~~~~

**型**: ``Boolean`` **デフォルト**: ``false``

デフォルトでは、ISSN の値として小文字の 'x' で終わるものが有効とみなされます。このオプションを ``true`` に設定すると、大文字の 'X' のみが許可されます。

requireHyphen
~~~~~~~~~~~~~

**型**: ``Boolean`` **デフォルト**: ``false``

デフォルトでは、ISSN の値としてハイフンなしのものが有効とみなされます。このオプションを ``true`` に設定すると、ハイフン付きの ISSN のみが許可されます。

.. _`ISSN`: http://en.wikipedia.org/wiki/Issn

.. 2013/06/09 hidenorigoto 582125098bd8c4adc8bb20c70323c49aabc043e8
