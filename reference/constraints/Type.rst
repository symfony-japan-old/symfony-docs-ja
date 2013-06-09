Type
====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

検証対象の値が指定したデータ型であることを検証します。たとえば、変数が配列であることを確かめたい場合は、この制約の ``type`` オプションで ``array`` を指定して検証します。

+----------------+---------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`         |
+----------------+---------------------------------------------------------------------+
| オプション     | - :ref:`type<reference-constraint-type-type>`                       |
|                | - `message`_                                                        |
+----------------+---------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Type`           |
+----------------+---------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\TypeValidator`  |
+----------------+---------------------------------------------------------------------+

基本的な使い方
--------------

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                age:
                    - Type:
                        type: integer
                        message: The value {{ value }} is not a valid {{ type }}.

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Type(type="integer", message="The value {{ value }} is not a valid {{ type }}.")
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="age">
                <constraint name="Type">
                    <option name="type">integer</option>
                    <option name="message">The value {{ value }} is not a valid {{ type }}.</option>
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
                $metadata->addPropertyConstraint('age', new Assert\Type(array(
                    'type'    => 'integer',
                    'message' => 'The value {{ value }} is not a valid {{ type }}.',
                )));
            }
        }

オプション
----------

.. _reference-constraint-type-type:

type
~~~~

**型**: ``string`` [:ref:`default option<validation-default-option>`]

このオプションは必須で、完全修飾クラス名か、次のようなPHP の ``is_`` 関数群でチェックできる PHP のデータ型を指定します。

* `array <http://php.net/is_array>`_
* `bool <http://php.net/is_bool>`_
* `callable <http://php.net/is_callable>`_
* `float <http://php.net/is_float>`_
* `double <http://php.net/is_double>`_
* `int <http://php.net/is_int>`_
* `integer <http://php.net/is_integer>`_
* `long <http://php.net/is_long>`_
* `null <http://php.net/is_null>`_
* `numeric <http://php.net/is_numeric>`_
* `object <http://php.net/is_object>`_
* `real <http://php.net/is_real>`_
* `resource <http://php.net/is_resource>`_
* `scalar <http://php.net/is_scalar>`_
* `string <http://php.net/is_string>`_

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value should be of type {{ type }}``

検証対象のデータが指定したデータ型ではない場合に、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto 72a6d59266859764764ef148d4acd44087b86dfa
