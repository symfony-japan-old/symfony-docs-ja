EqualTo
=======

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

.. versionadded:: 2.3
    この制約は、2.3 以降で利用可能です。

検証対象の値が、オプションで指定した値と等しいかどうか検証します。値が等しくないことを検証するには、\ :doc:`/reference/constraints/NotEqualTo` を使います。

.. caution::
    
    この制約では、内部的に値の比較に ``==`` 演算子を使います。したがって、\ ``3`` と ``"3"`` は等しいと評価されます。\ ``===`` で比較したい場合は :doc:`/reference/constraints/IdenticalTo` 制約を使ってください。

+----------------+-----------------------------------------------------------------------+
| 適用先         | :ref:`プロパティ又はメソッド<validation-property-target>`             |
+----------------+-----------------------------------------------------------------------+
| オプション     | - `value`_                                                            |
|                | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\EqualTo`          |
+----------------+-----------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\EqualToValidator` |
+----------------+-----------------------------------------------------------------------+

基本的な使い方
--------------

``Person`` クラスの ``age`` が ``20`` と等しいことを検証したい場合、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/SocialBundle/Resources/config/validation.yml
        Acme\SocialBundle\Entity\Person:
            properties:
                age:
                    - EqualTo:
                        value: 20

    .. code-block:: php-annotations

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            /**
             * @Assert\EqualTo(
             *     value = 20
             * )
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/SocialBundle/Resources/config/validation.xml -->
        <class name="Acme\SocialBundle\Entity\Person">
            <property name="age">
                <constraint name="EqualTo">
                    <option name="value">20</option>
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
                $metadata->addPropertyConstraint('age', new Assert\EqualTo(array(
                    'value' => 20,
                )));
            }
        }

オプション
----------

.. include:: /reference/constraints/_comparison-value-option.rst.inc

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value should be equal to {{ compared_value }}``

値が等しくなかった場合に、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto 48ca4f0d89d01e421960a58e0bf7d68aa7be21d4
