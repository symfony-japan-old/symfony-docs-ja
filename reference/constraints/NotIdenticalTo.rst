NotIdenticalTo
==============

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

.. versionadded:: 2.3
    この制約はバージョン 2.3 以降で利用可能です。

検証対象の値が、オプションで指定した値と厳密に\ **一致しない**\ ことを検証します。厳密に一致することを検証するには :doc:`/reference/constraints/IdenticalTo` 制約を使います。

.. caution::
    
    この制約では、値の比較に内部的に ``!==`` 演算子が使われます。したがって ``3`` と ``"3"`` は等しくないとみなされます。\ ``!=`` 演算子で比較したい場合は :doc:`/reference/constraints/NotEqualTo` 制約を使ってください。

+----------------+-----------------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`                 |
+----------------+-----------------------------------------------------------------------------+
| オプション     | - `value`_                                                                  |
|                | - `message`_                                                                |
+----------------+-----------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\NotIdenticalTo`         |
+----------------+-----------------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\NotIdenticalToValidator`|
+----------------+-----------------------------------------------------------------------------+

基本的な使い方
--------------

``Person`` クラスの ``age`` の値が ``15`` と等しくなく、\ **数値ではない**\ ことを保証する場合、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/SocialBundle/Resources/config/validation.yml
        Acme\SocialBundle\Entity\Person:
            properties:
                age:
                    - NotIdenticalTo:
                        value: 15

    .. code-block:: php-annotations

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            /**
             * @Assert\NotIdenticalTo(
             *     value = 15
             * )
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/SocialBundle/Resources/config/validation.xml -->
        <class name="Acme\SocialBundle\Entity\Person">
            <property name="age">
                <constraint name="NotIdenticalTo">
                    <option name="value">15</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('age', new Assert\NotIdenticalTo(array(
                    'value' => 15,
                )));
            }
        }

オプション
----------

.. include:: /reference/constraints/_comparison-value-option.rst.inc

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value should not be identical to {{ compared_value_type }} {{ compared_value }}``

検証対象の値が指定した値と一致する場合に、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto f9019350a6603cac669d67aa2f8c855f50c7e82c
