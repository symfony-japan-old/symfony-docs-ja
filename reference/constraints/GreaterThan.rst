GreaterThan
===========

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

.. versionadded:: 2.3
    この制約はバージョン 2.3 以降で利用可能です。

検証対象の値が、オプションで指定した値よりも大きいことを検証します。指定した値以上であることを検証したい場合は :doc:`/reference/constraints/GreaterThanOrEqual` 制約を使います。指定した値より小さいことを検証するには :doc:`/reference/constraints/LessThan` 制約を使います。

+----------------+---------------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`               |
+----------------+---------------------------------------------------------------------------+
| オプション     | - `value`_                                                                |
|                | - `message`_                                                              |
+----------------+---------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\GreaterThan`          |
+----------------+---------------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\GreaterThanValidator` |
+----------------+---------------------------------------------------------------------------+

基本的な使い方
--------------

``Person`` クラスの ``age`` が ``18`` より大きいことを保証したい場合、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/SocialBundle/Resources/config/validation.yml
        Acme\SocialBundle\Entity\Person:
            properties:
                age:
                    - GreaterThan:
                        value: 18

    .. code-block:: php-annotations

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            /**
             * @Assert\GreaterThan(
             *     value = 18
             * )
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/SocialBundle/Resources/config/validation.xml -->
        <class name="Acme\SocialBundle\Entity\Person">
            <property name="age">
                <constraint name="GreaterThan">
                    <option name="value">18</option>
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
                $metadata->addPropertyConstraint('age', new Assert\GreaterThan(array(
                    'value' => 18,
                )));
            }
        }

オプション
----------

.. include:: /reference/constraints/_comparison-value-option.rst.inc

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value should be greater than {{ compared_value }}``

検証対象の値が指定した値よりも大きくない場合に、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto 4542e061694055e1470a0827be7241fdba864637
