IdenticalTo
===========

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

.. versionadded:: 2.3
    この制約はバージョン 2.3 以降で利用可能です。

検証対象の値が、オプションで指定した値と同一であることを検証します。指定した値と同一ではないことを検証したい場合は :doc:`/reference/constraints/NotIdenticalTo` 制約を使います。

.. caution::
    
    この制約では値の比較に ``===`` 演算子が使われます。したがって、\ ``3`` と ``"3"`` は同一とは\ *みなされません*\ 。\ ``==`` で比較したい場合は :doc:`/reference/constraints/EqualTo` 制約を使います。

+----------------+--------------------------------------------------------------------------+
| 適用先         | :ref:`プロパティ又はメソッド<validation-property-target>`                |
+----------------+--------------------------------------------------------------------------+
| オプション     | - `value`_                                                               |
|                | - `message`_                                                             |
+----------------+--------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\IdenticalTo`         |
+----------------+--------------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\IdenticalToValidator`|
+----------------+--------------------------------------------------------------------------+

基本的な使い方
--------------

``Person`` クラスの ``age`` が ``20`` に等しく、数値であることを保証したい場合、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/SocialBundle/Resources/config/validation.yml
        Acme\SocialBundle\Entity\Person:
            properties:
                age:
                    - IdenticalTo:
                        value: 20

    .. code-block:: php-annotations

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            /**
             * @Assert\IdenticalTo(
             *     value = 20
             * )
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/SocialBundle/Resources/config/validation.xml -->
        <class name="Acme\SocialBundle\Entity\Person">
            <property name="age">
                <constraint name="IdenticalTo">
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
                $metadata->addPropertyConstraint('age', new Assert\IdenticalTo(array(
                    'value' => 20,
                )));
            }
        }

オプション
----------

.. include:: /reference/constraints/_comparison-value-option.rst.inc

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value should be identical to {{ compared_value_type }} {{ compared_value }}``

検証対象の値が指定した値と同一ではない場合に、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto 4542e061694055e1470a0827be7241fdba864637
