NotEqualTo
==========

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

.. versionadded:: 2.3
    この制約はバージョン 2.3 以降で利用可能です。

検証対象の値が、オプションで指定した値と\ **等しくない**\ ことを検証します。値が等しいことを検証するには :doc:`/reference/constraints/EqualTo` 制約を使います。

.. caution::
    
    この制約では、内部的に値の比較に ``!=`` 演算子が使われます。したがって ``3`` と ``"3"`` は等しいとみなされます。\ ``!==`` 演算子で比較したい場合は :doc:`/reference/constraints/NotIdenticalTo` 制約を使ってください。

+----------------+-------------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`             |
+----------------+-------------------------------------------------------------------------+
| オプション     | - `value`_                                                              |
|                | - `message`_                                                            |
+----------------+-------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\NotEqualTo`         |
+----------------+-------------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\NotEqualToValidator`|
+----------------+-------------------------------------------------------------------------+

基本的な使い方
--------------

``Person`` クラスの ``age`` の値が ``15`` と等しくないことを保証するには、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/SocialBundle/Resources/config/validation.yml
        Acme\SocialBundle\Entity\Person:
            properties:
                age:
                    - NotEqualTo:
                        value: 15

    .. code-block:: php-annotations

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            /**
             * @Assert\NotEqualTo(
             *     value = 15
             * )
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/SocialBundle/Resources/config/validation.xml -->
        <class name="Acme\SocialBundle\Entity\Person">
            <property name="age">
                <constraint name="NotEqualTo">
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
                $metadata->addPropertyConstraint('age', new Assert\NotEqualTo(array(
                    'value' => 15,
                )));
            }
        }

オプション
----------

.. include:: /reference/constraints/_comparison-value-option.rst.inc

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value should not be equal to {{ compared_value }}``

検証対象の値が指定した値と等しかった場合に、このメセージが表示されます。

.. 2013/06/09 hidenorigoto 48ca4f0d89d01e421960a58e0bf7d68aa7be21d4
