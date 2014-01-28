.. index::
   single: フォーム; "inherit_data"オプション

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2014/01/28

"inherit_data"オプションを使用してコードの重複を減らす
======================================================

.. versionadded:: 2.3
    ``inherit_data`` オプションはSymfonyバージョン2.2までは ``virtual`` と呼ばれていました。

``inherit_data`` オプションは異なるエンティティ間に重複するフィールドが存在する場合に有効です。
例えば次のような2つのエンティティ ``Company`` と ``Customer`` があるとします::

    // src/Acme/HelloBundle/Entity/Company.php
    namespace Acme\HelloBundle\Entity;

    class Company
    {
        private $name;
        private $website;

        private $address;
        private $zipcode;
        private $city;
        private $country;
    }

.. code-block:: php

    // src/Acme/HelloBundle/Entity/Customer.php
    namespace Acme\HelloBundle\Entity;

    class Customer
    {
        private $firstName;
        private $lastName;

        private $address;
        private $zipcode;
        private $city;
        private $country;
    }

ご覧のとおり、それぞれのエンティティには重複する ``address``, ``zipcode``, ``city``, ``country`` フィールドが定義されています。

続いてこれらのエンティティのフォーム ``CompanyType`` と ``CustomerType`` を作成してみましょう。::

    // src/Acme/HelloBundle/Form/Type/CompanyType.php
    namespace Acme\HelloBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;

    class CompanyType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('name', 'text')
                ->add('website', 'text');
        }
    }

.. code-block:: php

    // src/Acme/HelloBundle/Form/Type/CustomerType.php
    namespace Acme\HelloBundle\Form\Type;

    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\Form\AbstractType;

    class CustomerType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('firstName', 'text')
                ->add('lastName', 'text');
        }
    }

重複する ``address``, ``zipcode``, ``city``, ``country`` を両方のフォームに定義するのではなく、別のフォームを新たに作り、そこに定義します。
そのフォームを仮に ``LocationType`` としましょう。::

    // src/Acme/HelloBundle/Form/Type/LocationType.php
    namespace Acme\HelloBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class LocationType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('address', 'textarea')
                ->add('zipcode', 'text')
                ->add('city', 'text')
                ->add('country', 'text');
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'inherit_data' => true
            ));
        }

        public function getName()
        {
            return 'location';
        }
    }

このフォームではいよいよ ``inherit_data`` を定義します。このオプションは親フォームのデータを継承することを許可します。
例えば先ほど定義した Company のフォームに埋め込むと、このフォームは ``Company`` インスタンスのプロパティを操作します。同様に Customer のフォームに埋め込んだ場合は ``Customer`` のプロパティが操作されるという訳です。簡単でしょう？

.. note::

    ``LocationType`` に ``inherit_data`` オプションを定義する代わりに、 ``$builder->add()`` した際の第3引数に直接データクラスを定義する事でも同様の効果が得られます。

早速最初に定義した2つのフォームに埋め込んでみましょう::

    // src/Acme/HelloBundle/Form/Type/CompanyType.php
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        // ...

        $builder->add('foo', new LocationType(), array(
            'data_class' => 'Acme\HelloBundle\Entity\Company'
        ));
    }

.. code-block:: php

    // src/Acme/HelloBundle/Form/Type/CustomerType.php
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        // ...

        $builder->add('bar', new LocationType(), array(
            'data_class' => 'Acme\HelloBundle\Entity\Customer'
        ));
    }

無事、重複するフィールドを再利用可能な別のフォームに定義することができました！
 
.. 2014/01/27 issei-m d6e20f0d0c1f6a5632bab5a3de934a3e51445c11
