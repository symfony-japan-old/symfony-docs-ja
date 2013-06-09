Collection
==========

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/7

この制約は、検証対象のデータがコレクション、つまり配列であるか、\ ``Traversable`` や ``ArrayAccess`` を実装したオブジェクトである場合に使います。コレクション中のキーごとに異なる制約を適用できます。たとえば、\ ``email`` キーの要素には ``Email`` 制約を適用し、\ ``inventory`` キーの要素には ``Range`` 制約を適用する、といったことが可能です。

また、コレクション中の任意のキーに対して存在することを確認したり、予期しないキーが存在しないことを確認したりできます。

+----------------+--------------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`              |
+----------------+--------------------------------------------------------------------------+
| オプション     | - `fields`_                                                              |
|                | - `allowExtraFields`_                                                    |
|                | - `extraFieldsMessage`_                                                  |
|                | - `allowMissingFields`_                                                  |
|                | - `missingFieldsMessage`_                                                |
+----------------+--------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Collection`          |
+----------------+--------------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\CollectionValidator` |
+----------------+--------------------------------------------------------------------------+

基本的な使い方
--------------

``Collection`` 制約を使うと、コレクション中のキーごとに別々の検証を行えます。次のように ``Author`` クラスに ``$profileData`` があるとします。

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    class Author
    {
        protected $profileData = array(
            'personal_email',
            'short_bio',
        );

        public function setProfileData($key, $value)
        {
            $this->profileData[$key] = $value;
        }
    }

``profileData`` 配列プロパティの ``personal_email`` 要素は有効なメールアドレス、\ ``short_bio`` 要素は空ではなく 100 文字以内であることを検証したい場合、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                profileData:
                    - Collection:
                        fields:
                            personal_email: Email
                            short_bio:
                                - NotBlank
                                - Length:
                                    max:   100
                                    maxMessage: Your short bio is too long!
                        allowMissingFields: true

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Collection(
             *     fields = {
             *         "personal_email" = @Assert\Email,
             *         "short_bio" = {
             *             @Assert\NotBlank(),
             *             @Assert\Length(
             *                 max = 100,
             *                 maxMessage = "Your bio is too long!"
             *             )
             *         }
             *     },
             *     allowMissingFields = true
             * )
             */
             protected $profileData = array(
                 'personal_email',
                 'short_bio',
             );
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="profileData">
                <constraint name="Collection">
                    <option name="fields">
                        <value key="personal_email">
                            <constraint name="Email" />
                        </value>
                        <value key="short_bio">
                            <constraint name="NotBlank" />
                            <constraint name="Length">
                                <option name="max">100</option>
                                <option name="maxMessage">Your bio is too long!</option>
                            </constraint>
                        </value>
                    </option>
                    <option name="allowMissingFields">true</option>
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
            private $options = array();

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('profileData', new Assert\Collection(array(
                    'fields' => array(
                        'personal_email' => new Assert\Email(),
                        'lastName' => array(
                            new Assert\NotBlank(),
                            new Assert\Length(array("max" => 100)),
                        ),
                    ),
                    'allowMissingFields' => true,
                )));
            }
        }

フィールドの存在と欠落
~~~~~~~~~~~~~~~~~~~~~~

Collection 制約によりコレクション中の個々の要素に割り当てられた制約が単純に検証されるだけではありません。デフォルトでは、制約で指定したキーがコレクションに存在しなかったり、想定していないキーが存在した場合、バリデーションエラーがスローされます。

コレクションからのキーの欠落や、想定しないキーの存在を許可したい場合は、\ `allowMissingFields`_ オプションや `allowExtraFields`_ オプションを指定します。上の例では ``allowMissingFields`` オプションが ``true`` に設定されているので、\ ``$personalData`` プロパティに ``personal_email`` 要素または ``short_bio`` 要素が存在しなくてもバリデーションエラーは発生しません。

.. versionadded:: 2.3
    Symfony 2.3 以降では、\ ``Required`` 制約および ``Optional`` 制約が 
    ``Symfony\Component\Validator\Constraints\`` 名前空間に移動されました。

フィールドにおける必須および任意の制約
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

コレクション中のフィールドに対する制約を、\ ``Required`` 制約または ``Optional`` 制約でラップすることができます。\ ``Required`` でラップするとフィールドの有無に関わらず制約が適用されます。\ ``Optional`` でラップすると、フィールドが存在する場合にのみ制約が適用されます。

たとえば、\ ``ProfileData`` 配列中の ``personal_email`` フィールドは空ではなく有効なメールアドレスであることを検証する一方、\ ``alternate_email`` フィールドの入力は任意で、値がある場合は有効なメールアドレスであることを検証したい場合、次のようにします。

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Collection(
             *     fields={
             *         "personal_email"  = @Assert\Required({@Assert\NotBlank, @Assert\Email}),
             *         "alternate_email" = @Assert\Optional(@Assert\Email),
             *     }
             * )
             */
             protected $profileData = array(
                 'personal_email',
             );
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            protected $profileData = array('personal_email');

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('profileData', new Assert\Collection(array(
                    'fields' => array(
                        'personal_email'  => new Assert\Required(array(new Assert\NotBlank(), new Assert\Email())),
                        'alternate_email' => new Assert\Optional(new Assert\Email()),
                    ),
                )));
            }
        }

``alternate_email`` に ``Optional`` を指定しているので、\ ``allowMissingFields`` オプションを ``true`` に設定しなくても、\ ``profileData`` 配列から ``alternate_email``
プロパティは省略することができます。
``personal_email`` フィールドには ``Required`` を指定しているので、配列に ``personal_email`` フィールドが存在しなくても ``NotBlank`` 制約が適用され、検証は失敗します。

オプション
----------

fields
~~~~~~

**型**: ``array`` [:ref:`default option<validation-default-option>`]

This option is required, and is an associative array defining all of the
keys in the collection and, for each key, exactly which validator(s) should
be executed against that element of the collection.

allowExtraFields
~~~~~~~~~~~~~~~~

**型**: ``Boolean`` **デフォルト**: false

`fields`_ オプションにない要素が検証対象のコレクションに存在した場合、このオプションが ``false`` に設定されていると、バリデーションエラーが返されます。\ ``true`` に設定すると、エラーになりません。

extraFieldsMessage
~~~~~~~~~~~~~~~~~~

**型**: ``Boolean`` **デフォルト**: ``The fields {{ fields }} were not expected``

検証対象のコレクションに予期しないフィールドが見つかり、\ `allowExtraFields`_ が ``false`` に設定されている場合に、このメッセージが表示されます。allowMissingFields

allowMissingFields
~~~~~~~~~~~~~~~~~~

**型**: ``Boolean`` **デフォルト**: false

`fields`_ オプションに指定された要素が検証対象のコレクションに存在しない場合、このオプションが ``false``  に設定されていると、バリデーションエラーが返されます。\ ``true`` に設定すると、\ `fields`_ オプションにしていしたフィールドが検証対象のコレクションに存在しなくてもエラーになりません。

missingFieldsMessage
~~~~~~~~~~~~~~~~~~~~

**型**: ``Boolean`` **デフォルト**: ``The fields {{ fields }} are missing``

検証対象のコレクションに期待するフィールドが存在せず、\ `allowMissingFields`_ が ``false`` に設定されている場合に、このメッセージが表示されます。

.. 2013/06/07 hidenorigoto bf6751719dc84ba61ccf737a496c49f4e830d7cc
