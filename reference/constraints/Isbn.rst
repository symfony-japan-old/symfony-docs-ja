Isbn
====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

.. versionadded:: 2.3
    この制約はバージョン 2.3 以降で利用可能です。

この制約は、ISBN (International Standard Book Numbers) 番号であることを検証します。ISBN-10、ISBN-13のいずれかまたは両方で検証できます。

+----------------+----------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`          |
+----------------+----------------------------------------------------------------------+
| オプション     | - `isbn10`_                                                          |
|                | - `isbn13`_                                                          |
|                | - `isbn10Message`_                                                   |
|                | - `isbn13Message`_                                                   |
|                | - `bothIsbnMessage`_                                                 |
+----------------+----------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Isbn`            |
+----------------+----------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\IsbnValidator`   |
+----------------+----------------------------------------------------------------------+

基本的な使い方
--------------

``Isbn`` バリデーターを使うには、オブジェクトの ISBN 番号を格納するプロパティやメソッドに単純に適用します。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BookcaseBunlde/Resources/config/validation.yml
        Acme\BookcaseBunlde\Entity\Book:
            properties:
                isbn:
                    - Isbn:
                        isbn10: true
                        isbn13: true
                        bothIsbnMessage: This value is neither a valid ISBN-10 nor a valid ISBN-13.

    .. code-block:: php-annotations

        // src/Acme/BookcaseBunlde/Entity/Book.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Book
        {
            /**
             * @Assert\Isbn(
             *     isbn10 = true,
             *     isbn13 = true,
             *     bothIsbnMessage = "This value is neither a valid ISBN-10 nor a valid ISBN-13."
             * )
             */
            protected $isbn;
        }

    .. code-block:: xml

        <!-- src/Acme/BookcaseBunlde/Resources/config/validation.xml -->
        <class name="Acme\BookcaseBunlde\Entity\Book">
            <property name="isbn">
                <constraint name="Isbn">
                    <option name="isbn10">true</option>
                    <option name="isbn13">true</option>
                    <option name="bothIsbnMessage">This value is neither a valid ISBN-10 nor a valid ISBN-13.</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BookcaseBunlde/Entity/Book.php
        namespace Acme\BookcaseBunlde\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Book
        {
            protected $isbn;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('isbn', new Assert\Isbn(array(
                    'isbn10'          => true,
                    'isbn13'          => true,
                    'bothIsbnMessage' => 'This value is neither a valid ISBN-10 nor a valid ISBN-13.'
                )));
            }
        }

利用可能なオプション
--------------------

isbn10
~~~~~~

**型**: ``boolean`` [:ref:`default option<validation-default-option>`]

このオプションは必須で、\ ``true`` に設定すると検証対象の値が ISBN-10 コードかどうかチェックします。

isbn13
~~~~~~

**型**: ``boolean`` [:ref:`default option<validation-default-option>`]

このオプションは必須で、\ ``true`` に設定すると検証対象の値が ISBN-13 コードかどうかチェックします。

isbn10Message
~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``This value is not a valid ISBN-10.``

`isbn10`_ オプションが ``true`` で ISBN-10 チェックに失敗した場合に、このメッセージが表示されます。

isbn13Message
~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``This value is not a valid ISBN-13.``

`isbn13`_ オプションが ``true`` で ISBN-13 チェックに失敗した場合に、このメッセージが表示されます。

bothIsbnMessage
~~~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``This value is neither a valid ISBN-10 nor a valid ISBN-13.``

`isbn10`_ オプションと `isbn13`_ オプションの両方が ``true`` でいずれのチェックにも失敗した場合に、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto 4ec619abd50aac8672bd3c2e3cb19612d643dd70
