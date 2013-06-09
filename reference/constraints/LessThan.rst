LessThan
========

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

.. versionadded:: 2.3
    この制約はバージョン 2.3 以降で利用可能です。

検証対象の値が、オプションで指定した値未満であることを検証します。指定した値以下であることを検証したい場合は :doc:`/reference/constraints/LessThanOrEqual` 制約を使います。指定した値より大きいことを検証するには :doc:`/reference/constraints/GreaterThan` 制約を使います。

+----------------+------------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`            |
+----------------+------------------------------------------------------------------------+
| オプション     | - `value`_                                                             |
|                | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\LessThan`          |
+----------------+------------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\LessThanValidator` |
+----------------+------------------------------------------------------------------------+

基本的な使い方
--------------

``Person`` クラスの ``age`` の値が ``80`` 未満であることを保証したい場合、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/SocialBundle/Resources/config/validation.yml
        Acme\SocialBundle\Entity\Person:
            properties:
                age:
                    - LessThan:
                        value: 80

    .. code-block:: php-annotations

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            /**
             * @Assert\LessThan(
             *     value = 80
             * )
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/SocialBundle/Resources/config/validation.xml -->
        <class name="Acme\SocialBundle\Entity\Person">
            <property name="age">
                <constraint name="LessThan">
                    <option name="value">80</option>
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
                $metadata->addPropertyConstraint('age', new Assert\LessThan(array(
                    'value' => 80,
                )));
            }
        }

オプション
----------

.. include:: /reference/constraints/_comparison-value-option.rst.inc

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value should be less than {{ compared_value }}``

検証対象の文字列の長さが指定した長さより短くなかった場合に、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto 4542e061694055e1470a0827be7241fdba864637
