Regex
=====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

検証対象の値が正規表現にマッチするかどうか検証します。

+----------------+-----------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`           |
+----------------+-----------------------------------------------------------------------+
| オプション     | - `pattern`_                                                          |
|                | - `match`_                                                            |
|                | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Regex`            |
+----------------+-----------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\RegexValidator`   |
+----------------+-----------------------------------------------------------------------+

基本的な使い方
--------------

``description`` フィールドがあり、有効な文字から始まることを検証したいとします。この検証用の正規表現は ``/^\w+/`` で、文字列の先頭に 1 文字以上のアルファベットがあることを示しています。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                description:
                    - Regex: "/^\w+/"

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Regex("/^\w+/")
             */
            protected $description;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="description">
                <constraint name="Regex">
                    <option name="pattern">/^\w+/</option>
                </constraint>
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
                $metadata->addPropertyConstraint('description', new Assert\Regex(array(
                    'pattern' => '/^\w+/',
                )));
            }
        }

`match`_ オプションを ``false`` に設定すると、正規表現に\ **マッチしない**\ ことを検証できます。次の例では、\ ``firstName`` フィールドに数字が含まれていないことを検証し、エラーの場合にはカスタムメッセージを表示するよう指定しています。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - Regex:
                        pattern: "/\d/"
                        match:   false
                        message: Your name cannot contain a number

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Regex(
             *     pattern="/\d/",
             *     match=false,
             *     message="Your name cannot contain a number"
             * )
             */
            protected $firstName;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="firstName">
                <constraint name="Regex">
                    <option name="pattern">/\d/</option>
                    <option name="match">false</option>
                    <option name="message">Your name cannot contain a number</option>
                </constraint>
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
                $metadata->addPropertyConstraint('firstName', new Assert\Regex(array(
                    'pattern' => '/\d/',
                    'match'   => false,
                    'message' => 'Your name cannot contain a number',
                )));
            }
        }

オプション
----------

pattern
~~~~~~~

**型**: ``string`` [:ref:`default option<validation-default-option>`]

このオプションは必須で、入力文字列を検査するための正規表現を指定します。入力文字列が正規表現にマッチしない場合、デフォルトでは検証は失敗となります。正規表現のマッチングには :phpfunction:`preg_match` PHP 関数が使われます。\ `match`_ オプションを ``false`` に設定すると、入力文字列が正規表現にマッチした場合に検証が失敗となります。

match
~~~~~

**型**: ``Boolean`` **デフォルト**: ``true``

``true`` または何も指定しない場合、入力文字列が `pattern`_ オプションの正規表現に\ **マッチした**\ 場合、検証にパスします。\ ``false`` に設定すると、入力文字列がttern`_ オプションの正規表現に\ **マッチしない**\ 場合にのみ検証にパスします。

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value is not valid``

バリデーターの検証に失敗した場合に、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto 131a287b3d371cdc37ff1c3c3dbcfe9dd35432a3
