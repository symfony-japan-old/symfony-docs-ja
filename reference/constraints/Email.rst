Email
=====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

値が有効なメールアドレスであることを検証します。検証対象の値は、バリデーション前に文字列にキャストされます。

+----------------+---------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`         |
+----------------+---------------------------------------------------------------------+
| オプション     | - `message`_                                                        |
|                | - `checkMX`_                                                        |
|                | - `checkHost`_                                                      |
+----------------+---------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Email`          |
+----------------+---------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\EmailValidator` |
+----------------+---------------------------------------------------------------------+

基本的な使い方
--------------

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                email:
                    - Email:
                        message: The email "{{ value }}" is not a valid email.
                        checkMX: true

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Email(
             *     message = "The email '{{ value }}' is not a valid email.",
             *     checkMX = true
             * )
             */
             protected $email;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="email">
                    <constraint name="Email">
                        <option name="message">The email "{{ value }}" is not a valid email.</option>
                        <option name="checkMX">true</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('email', new Assert\Email(array(
                    'message' => 'The email "{{ value }}" is not a valid email.',
                    'checkMX' => true,
                )));
            }
        }


オプション
----------

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value is not a valid email address``

検証対象の文字列が有効なメールアドレスではない場合に、このメッセージが表示されます。

checkMX
~~~~~~~

**型**: ``Boolean`` **デフォルト**: ``false``

このオプションを ``true`` に設定すると、検証対象のメールアドレスのホスト部が DNS の MX レコードとして有効かどうか :phpfunction:`checkdnsrr` PHP 関数を使ってチェックされます。

checkHost
~~~~~~~~~

**型**: ``Boolean`` **デフォルト**: ``false``

このオプションを ``true`` に設定すると、検証対象のメールアドレスのホスト部が、DNS の MX または A または AAAA レコードとして有効かどうか :phpfunction:`checkdnsrr` PHP 関数を使ってチェックされます。

.. 2011/07/23 yanchi 36a165e88363fd6e5b5eb0ae712303dd362545be
.. 2013/06/09 hidenorigoto 4346f75f05a5ee010d0148ea251e99c7f6a02c38
