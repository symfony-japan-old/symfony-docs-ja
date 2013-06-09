Null
====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

検証対象の値が厳密に ``null`` であることを検証します。単純に空 (空文字列または ``null``) であることを検証するには :doc:`/reference/constraints/Blank`
制約を使います。プロパティが null ではないことを検証するには :doc:`/reference/constraints/NotNull` 制約を使います。

+----------------+-----------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`           |
+----------------+-----------------------------------------------------------------------+
| オプション     | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Null`             |
+----------------+-----------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\NullValidator`    |
+----------------+-----------------------------------------------------------------------+

基本的な使い方
--------------

``Author`` クラスの ``firstName`` プロパティが厳密に ``null`` であることを保証するには、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - 'Null': ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Null()
             */
            protected $firstName;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="firstName">
                <constraint name="Null" />
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
                $metadata->addPropertyConstraint('firstName', Assert\Null());
            }
        }

オプション
----------

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value should be null``

検証対象の値が ``null`` ではなかった場合に、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto 72a6d59266859764764ef148d4acd44087b86dfa

