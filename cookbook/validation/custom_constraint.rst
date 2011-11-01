.. index::
   single: Validation; Custom constraints

カスタムバリデーション制約の作成方法
--------------------------------------------

ベースの制約クラス :class:`Symfony\\Component\\Validator\\Constraint` を拡張して、カスタム制約を作成することができます。制約のオプションは、制約クラスにパブリックなプロパティとして表示されます。例えば、 :doc:`Url</reference/constraints/Url>` 制約には、 ``message`` と ``protocols`` プロパティがあります。

.. code-block:: php

    namespace Symfony\Component\Validator\Constraints;
    
    use Symfony\Component\Validator\Constraint;

    /**
     * @Annotation
     */
    class Url extends Constraint
    {
        public $message = 'This value is not a valid URL';
        public $protocols = array('http', 'https', 'ftp', 'ftps');
    }

.. note::

    ``@Annotation`` タグは、アノテーションを介したクラスを使用可能にするため、この新しい制約に必要です。

このように、制約クラスは、とてもミニマルな構成になっています。実際のバリデーションは、他の "constraint validator" クラスによって実行されます。制約バリデータクラスは、制約の ``validatedBy()`` メソッドにより指定されます。 ``validatedBy()`` メソッドには、次のように簡単なデフォルトのロジックがあります。
::

.. code-block:: php

    // in the base Symfony\Component\Validator\Constraint class
    public function validatedBy()
    {
        return get_class($this).'Validator';
    }

つまり、 ``MyConstraint`` のようなカスタム ``Constraint`` を作成すると、 Symfony2 は自動的に他のクラスである ``MyConstraintValidator`` を参照し、実際のバリデーションを行います。

バリデーションクラスもシンプルで、 ``isValid`` メソッドのみが必要になります。 ``NotBlankValidator`` を例にして見てみましょう。

.. code-block:: php

    class NotBlankValidator extends ConstraintValidator
    {
        public function isValid($value, Constraint $constraint)
        {
            if (null === $value || '' === $value) {
                $this->setMessage($constraint->message);

                return false;
            }

            return true;
        }
    }

依存状態のある制約バリデー
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

制約バリデータがデータベース接続など依存を持っている際には、DI コンテナのサービスとして設定される必要があります。このサービスは、 ``validator.constraint_validator`` タグと ``alias`` 属性を含む必要があります。

.. configuration-block::

    .. code-block:: yaml

        services:
            validator.unique.your_validator_name:
                class: Fully\Qualified\Validator\Class\Name
                tags:
                    - { name: validator.constraint_validator, alias: alias_name }

    .. code-block:: xml

        <service id="validator.unique.your_validator_name" class="Fully\Qualified\Validator\Class\Name">
            <argument type="service" id="doctrine.orm.default_entity_manager" />
            <tag name="validator.constraint_validator" alias="alias_name" />
        </service>

    .. code-block:: php

        $container
            ->register('validator.unique.your_validator_name', 'Fully\Qualified\Validator\Class\Name')
            ->addTag('validator.constraint_validator', array('alias' => 'alias_name'))
        ;

これで制約クラスは、このエイリアスを適切なバリデータに参照することができました。
::

    public function validatedBy()
    {
        return 'alias_name';
    }

.. 2011/11/02 ganchiku a2770369a8b36ba8e5ac67d0f8c395a5d11c4c11

