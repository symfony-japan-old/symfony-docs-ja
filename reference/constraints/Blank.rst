Blank
=====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/8

値が空かどうか、つまり空文字列または ``null`` と等しいかどうかを検証します。厳密に ``null`` かどうかを検証するには :doc:`/reference/constraints/Null` 制約を使ってください。空では\ *ない*\ ことを検証するには、\ :doc:`/reference/constraints/NotBlank` 制約を使ってください。

+----------------+-----------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`           |
+----------------+-----------------------------------------------------------------------+
| オプション     | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Blank`            |
+----------------+-----------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\BlankValidator`   |
+----------------+-----------------------------------------------------------------------+

基本的な使い方
--------------

何らかの理由で ``Author`` クラスの ``firstName`` プロパティが空であることを検証したい場合、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - Blank: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Blank()
             */
            protected $firstName;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="firstName">
                <constraint name="Blank" />
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', new Assert\Blank());
            }
        }

オプション
----------

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value should be blank``

値が空ではなかった場合にこの文字列が表示されます。

.. 2013/06/07 hidenorigoto 3439aaf015ce6da557b582188c5676fe79b583de
