NotBlank
========

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

値が空ではないこと、つまり空文字列ではないか、\ ``null`` ではないことを検証します。厳密に ``null`` と等しくないことだけを検証するには :doc:`/reference/constraints/NotNull` 制約を使ってください。

+----------------+------------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`            |
+----------------+------------------------------------------------------------------------+
| オプション     | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\NotBlank`          |
+----------------+------------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\NotBlankValidator` |
+----------------+------------------------------------------------------------------------+

基本的な使い方
--------------

``Author`` クラスの ``firstName`` プロパティが空文字列ではないことを保証する場合、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - NotBlank: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank()
             */
            protected $firstName;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="firstName">
                <constraint name="NotBlank" />
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
                $metadata->addPropertyConstraint('firstName', new Assert\NotBlank());
            }
        }

オプション
----------

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value should not be blank``

検証対象の値が空文字列だった場合に、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto 9cabf0df7098a47e28515000b90f9c7446c17d6e
